#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/delaunay/delaunay.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input point cloud / mesh file");
    CmdLine::declare_arg("output", "delaunay.meshb", "Path to the output mesh file");

    std::vector<std::string> filenames;
    if(!CmdLine::parse(argc, argv, filenames, "input_points")) {
        return 1;
    }

    std::string input_file = CmdLine::get_arg("input");
    if(input_file.empty() && !filenames.empty()) {
        input_file = filenames[0];
    }
    
    if(input_file.empty()) {
        Logger::err("App") << "Please provide an input mesh/points file." << std::endl;
        return 1;
    }

    std::string output_file = CmdLine::get_arg("output");

    try {
        Stopwatch W("Delaunay");
        Mesh M_in, M_out;

        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M_in)) return 1;

        if(M_in.vertices.nb() < 4) {
            Logger::err("Delaunay") << "Need at least 4 points for 3D Delaunay." << std::endl;
            return 1;
        }

        Logger::out("Delaunay") << "Computing 3D Delaunay triangulation for " 
                                << M_in.vertices.nb() << " vertices." << std::endl;

        // Copy vertices to output mesh
        M_out.copy(M_in);
        M_out.facets.clear();
        M_out.cells.clear();
        M_out.edges.clear();

        // Create Delaunay triangulator
        SmartPointer<Delaunay> delaunay = Delaunay::create(3, "default");
        delaunay->set_vertices(M_out.vertices.nb(), M_out.vertices.point_ptr(0));

        // Create volumetric cells in the output mesh
        vector<index_t> tets;
        for(index_t c = 0; c < delaunay->nb_cells(); ++c) {
            index_t v0 = delaunay->cell_vertex(c, 0);
            index_t v1 = delaunay->cell_vertex(c, 1);
            index_t v2 = delaunay->cell_vertex(c, 2);
            index_t v3 = delaunay->cell_vertex(c, 3);
            if(v0 != NO_INDEX && v1 != NO_INDEX && v2 != NO_INDEX && v3 != NO_INDEX) {
                tets.push_back(v0);
                tets.push_back(v1);
                tets.push_back(v2);
                tets.push_back(v3);
            }
        }
        M_out.cells.assign_tet_mesh(tets, false);
        M_out.cells.connect();

        Logger::out("Delaunay") << "Generated " << M_out.cells.nb() << " tetrahedra." << std::endl;

        Logger::out("I/O") << "Saving " << output_file << "..." << std::endl;
        if(!mesh_save(M_out, output_file)) return 1;
    }
    catch(const std::exception& e) {
        Logger::err("App") << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
