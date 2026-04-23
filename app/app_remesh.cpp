#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_remesh.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input mesh file");
    CmdLine::declare_arg("output", "remeshed.meshb", "Path to the output mesh file");
    CmdLine::declare_arg("vertices", 10000, "Target number of vertices");
    CmdLine::declare_arg("dim", 3, "Dimension (3 for isotropic, 6 for anisotropic)");
    CmdLine::declare_arg("iter", 3, "Number of remeshing iterations (Lloyd)");

    std::vector<std::string> filenames;
    if(!CmdLine::parse(argc, argv, filenames, "input_mesh")) {
        return 1;
    }

    std::string input_file = CmdLine::get_arg("input");
    if(input_file.empty() && !filenames.empty()) {
        input_file = filenames[0];
    }
    
    if(input_file.empty()) {
        Logger::err("App") << "Please provide an input mesh file." << std::endl;
        return 1;
    }

    std::string output_file = CmdLine::get_arg("output");
    GEO::index_t nb_vertices = CmdLine::get_arg_uint("vertices");
    GEO::index_t dim = CmdLine::get_arg_uint("dim");
    GEO::index_t iter = CmdLine::get_arg_uint("iter");

    try {
        Stopwatch W("Remeshing");
        Mesh M_in, M_out;

        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M_in)) {
            return 1;
        }

        Logger::out("Remesh") << "Target vertices: " << nb_vertices << std::endl;
        Logger::out("Remesh") << "Dimension: " << dim << std::endl;

        remesh_smooth(M_in, M_out, nb_vertices, dim, iter);

        Logger::out("Remesh") << "Output has " << M_out.vertices.nb() << " vertices and "
                              << M_out.facets.nb() << " facets." << std::endl;

        Logger::out("I/O") << "Saving " << output_file << "..." << std::endl;
        if(!mesh_save(M_out, output_file)) {
            return 1;
        }
    }
    catch(const std::exception& e) {
        Logger::err("App") << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
