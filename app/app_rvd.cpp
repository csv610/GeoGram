#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_geometry.h>
#include <geogram/delaunay/delaunay.h>
#include <geogram/voronoi/RVD.h>

/**
 * A C++ CLI application to compute Restricted Voronoi Diagrams (RVD).
 * It intersects a Voronoi diagram (defined by seeds) with a background mesh.
 */
int main(int argc, char** argv) {
    using namespace GEO;

    // 1. Initialize Geogram
    initialize(GEOGRAM_INSTALL_ALL);

    // 2. Configure the command line
    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the background mesh (restriction)");
    CmdLine::declare_arg("seeds", "", "Path to the seed points file (mesh or xyz)");
    CmdLine::declare_arg("output", "rvd.meshb", "Path to the output RVD cells mesh");
    CmdLine::declare_arg("nb_seeds", 100, "Number of random seeds if 'seeds' is not provided");
    CmdLine::declare_arg("volumetric", false, "Compute volumetric RVD (if input has tets)");

    std::vector<std::string> filenames;
    if(!CmdLine::parse(argc, argv, filenames, "input_mesh")) {
        return 1;
    }

    std::string input_file = CmdLine::get_arg("input");
    if(input_file.empty() && !filenames.empty()) {
        input_file = filenames[0];
    }
    
    if(input_file.empty()) {
        Logger::err("RVD") << "Please provide an input background mesh." << std::endl;
        return 1;
    }

    std::string seeds_file = CmdLine::get_arg("seeds");
    std::string output_file = CmdLine::get_arg("output");
    bool volumetric = CmdLine::get_arg_bool("volumetric");

    try {
        Stopwatch W("Total RVD time");
        Mesh M_bg;
        Mesh M_seeds;

        // 3. Load background mesh
        Logger::out("I/O") << "Loading background mesh " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M_bg)) return 1;

        // 4. Load or generate seeds
        if(!seeds_file.empty()) {
            Logger::out("I/O") << "Loading seeds from " << seeds_file << "..." << std::endl;
            if(!mesh_load(seeds_file, M_seeds)) return 1;
        } else {
            index_t nb_seeds = CmdLine::get_arg_uint("nb_seeds");
            Logger::out("RVD") << "Generating " << nb_seeds << " random seeds..." << std::endl;
            M_seeds.vertices.create_vertices(nb_seeds);
            
            double xyzmin[3], xyzmax[3];
            get_bbox(M_bg, xyzmin, xyzmax);
            
            for(index_t v=0; v<nb_seeds; ++v) {
                double* p = M_seeds.vertices.point_ptr(v);
                p[0] = xyzmin[0] + (xyzmax[0]-xyzmin[0]) * double(rand())/double(RAND_MAX);
                p[1] = xyzmin[1] + (xyzmax[1]-xyzmin[1]) * double(rand())/double(RAND_MAX);
                p[2] = xyzmin[2] + (xyzmax[2]-xyzmin[2]) * double(rand())/double(RAND_MAX);
            }
        }

        if(M_seeds.vertices.nb() == 0) {
            Logger::err("RVD") << "No seeds provided or generated." << std::endl;
            return 1;
        }

        // 5. Compute Delaunay of seeds
        Logger::out("RVD") << "Computing Delaunay of " << M_seeds.vertices.nb() << " seeds..." << std::endl;
        SmartPointer<Delaunay> delaunay = Delaunay::create(3, "default");
        delaunay->set_vertices(M_seeds.vertices.nb(), M_seeds.vertices.point_ptr(0));

        // 6. Compute Restricted Voronoi Diagram
        Logger::out("RVD") << "Computing Restricted Voronoi Diagram..." << std::endl;
        Mesh M_out;
        
        SmartPointer<RestrictedVoronoiDiagram> rvd = RestrictedVoronoiDiagram::create(delaunay, &M_bg);
        rvd->set_volumetric(volumetric);
        
        // Simple API to compute the RVD and store it in a mesh
        // parameters: result_mesh, nb_subdivisions, cell_borders, integration_simplices
        rvd->compute_RVD(M_out, 0, false, false);

        Logger::out("App") << "RVD has " << M_out.vertices.nb() << " vertices and "
                              << M_out.facets.nb() << " facets." << std::endl;

        // 7. Save result
        Logger::out("I/O") << "Saving RVD cells to " << output_file << "..." << std::endl;
        if(!mesh_save(M_out, output_file)) return 1;

        Logger::out("RVD") << "Done." << std::endl;
    }
    catch(const std::exception& e) {
        Logger::err("RVD") << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
