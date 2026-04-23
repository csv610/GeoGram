#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/points/co3ne.h>
#include <geogram/third_party/PoissonRecon/poisson_geogram.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input point cloud file");
    CmdLine::declare_arg("output", "reconstructed.meshb", "Path to the output mesh file");
    CmdLine::declare_arg("method", "co3ne", "Method: co3ne or poisson");
    
    // Co3Ne specific args
    CmdLine::declare_arg("nb_neighbors", 20, "Number of neighbors for normal/local reconstruction");
    CmdLine::declare_arg("nb_iterations", 1, "Number of smoothing iterations (Co3Ne)");
    CmdLine::declare_arg("radius", 0.0, "Search radius for triangles (0.0 for auto)");
    
    // Poisson specific args
    CmdLine::declare_arg("depth", 8, "Octree depth (Poisson)");

    std::vector<std::string> filenames;
    if(!CmdLine::parse(argc, argv, filenames, "input_points")) {
        return 1;
    }

    std::string input_file = CmdLine::get_arg("input");
    if(input_file.empty() && !filenames.empty()) {
        input_file = filenames[0];
    }
    
    if(input_file.empty()) {
        Logger::err("App") << "Please provide an input point cloud file." << std::endl;
        return 1;
    }

    std::string output_file = CmdLine::get_arg("output");
    std::string method = CmdLine::get_arg("method");

    try {
        Stopwatch W("Reconstruction");
        Mesh M_in, M_out;

        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M_in)) return 1;

        if(M_in.vertices.nb() == 0) {
            Logger::err("App") << "Input point cloud is empty." << std::endl;
            return 1;
        }

        if (method == "co3ne") {
            index_t nb_neighbors = CmdLine::get_arg_uint("nb_neighbors");
            index_t nb_iterations = CmdLine::get_arg_uint("nb_iterations");
            double radius = CmdLine::get_arg_double("radius");
            
            Logger::out("Reconstruct") << "Method: Co3Ne" << std::endl;
            Co3Ne_smooth_and_reconstruct(M_in, nb_neighbors, nb_iterations, radius);
            M_out.copy(M_in);
        } else if (method == "poisson") {
            index_t depth = CmdLine::get_arg_uint("depth");
            index_t nb_neighbors = CmdLine::get_arg_uint("nb_neighbors");
            
            Logger::out("Reconstruct") << "Method: Poisson (depth=" << depth << ")" << std::endl;
            
            // 1. Compute normals (Poisson requires them)
            Logger::out("Reconstruct") << "Computing normals..." << std::endl;
            if(!Co3Ne_compute_normals(M_in, nb_neighbors, true)) {
                return 1;
            }
            
            // 2. Run Poisson Reconstruction
            PoissonReconstruction poisson;
            poisson.set_depth(depth);
            poisson.reconstruct(&M_in, &M_out);
        } else {
            Logger::err("App") << "Unknown method: " << method << std::endl;
            return 1;
        }

        Logger::out("Reconstruct") << "Output has " << M_out.facets.nb() << " facets." << std::endl;

        Logger::out("I/O") << "Saving " << output_file << "..." << std::endl;
        if(!mesh_save(M_out, output_file)) return 1;
    }
    catch(const std::exception& e) {
        Logger::err("App") << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
