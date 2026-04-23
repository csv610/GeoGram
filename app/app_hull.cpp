#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_convex_hull.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input point set file");
    CmdLine::declare_arg("output", "hull.meshb", "Path to the output hull mesh file");
    CmdLine::declare_arg("dim", 3, "Dimension (2 or 3)");

    std::vector<std::string> filenames;
    if(!CmdLine::parse(argc, argv, filenames, "input_points")) {
        return 1;
    }

    std::string input_file = CmdLine::get_arg("input");
    if(input_file.empty() && !filenames.empty()) {
        input_file = filenames[0];
    }

    if(input_file.empty()) {
        Logger::err("App") << "Please provide an input point file." << std::endl;
        return 1;
    }

    std::string output_file = CmdLine::get_arg("output");
    index_t dim = CmdLine::get_arg_uint("dim");

    try {
        Stopwatch W("Convex Hull");
        Mesh M;

        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M)) {
            return 1;
        }

        Logger::out("Hull") << "Input: " << M.vertices.nb() << " vertices" << std::endl;

        if(dim == 2) {
            Logger::out("Hull") << "Computing 2D convex hull..." << std::endl;
            compute_convex_hull_2d(M);
        } else {
            Logger::out("Hull") << "Computing 3D convex hull..." << std::endl;
            compute_convex_hull_3d(M);
        }

        Logger::out("Hull") << "Result: " << M.vertices.nb() << " vertices, "
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