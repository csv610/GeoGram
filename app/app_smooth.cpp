#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_smoothing.h>
#include <geogram/mesh/mesh_geometry.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input mesh file");
    CmdLine::declare_arg("output", "smoothed.meshb", "Path to the output mesh file");
    CmdLine::declare_arg("method", "laplacian", "Smoothing method (laplacian, optimize)");
    CmdLine::declare_arg("iterations", 10, "Number of iterations (for Laplacian)");
    CmdLine::declare_arg("normals_only", false, "Smooth only normals (for Laplacian)");

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
    std::string method = CmdLine::get_arg("method");
    index_t iterations = CmdLine::get_arg_uint("iterations");
    bool normals_only = CmdLine::get_arg_bool("normals_only");

    try {
        Stopwatch W("Mesh Smoothing");
        Mesh M;

        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M)) {
            return 1;
        }

        Logger::out("Smoothing") << "Input: " << M.vertices.nb() << " vertices, "
                                << M.facets.nb() << " facets" << std::endl;

        if(method == "laplacian") {
            Logger::out("Smoothing") << "Method: Laplacian" << std::endl;
            Logger::out("Smoothing") << "Iterations: " << iterations << std::endl;
            Logger::out("Smoothing") << "Normals only: " << (normals_only ? "yes" : "no") << std::endl;

            if(M.vertices.dimension() < 6) {
                Logger::out("Smoothing") << "Computing normals (required for Laplacian)..." << std::endl;
                compute_normals(M);
            }
            simple_Laplacian_smooth(M, iterations, normals_only);
        } else if(method == "optimize") {
            Logger::out("Smoothing") << "Method: Optimize (Tetrahedral-based)" << std::endl;
            mesh_smooth(M);
        } else {
            Logger::err("App") << "Unknown method: " << method << std::endl;
            return 1;
        }

        Logger::out("Smoothing") << "Result: " << M.vertices.nb() << " vertices, "
                                << M.facets.nb() << " facets" << std::endl;

        Logger::out("I/O") << "Saving " << output_file << "..." << std::endl;
        if(!mesh_save(M, output_file)) {
            return 1;
        }

        Logger::out("App") << "Done." << std::endl;
    }
    catch(const std::exception& e) {
        Logger::err("App") << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}