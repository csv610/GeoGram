#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_fill_holes.h>
#include <geogram/mesh/mesh_geometry.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input mesh file");
    CmdLine::declare_arg("output", "filled.meshb", "Path to the output mesh file");
    CmdLine::declare_arg("max_area", 0.0, "Maximum hole area to fill (0=unlimited)");
    CmdLine::declare_arg("max_edges", 0, "Maximum number of edges around hole (0=unlimited)");
    CmdLine::declare_arg("repair", true, "Repair facets after filling");

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
    double max_area = CmdLine::get_arg_double("max_area");
    index_t max_edges = CmdLine::get_arg_uint("max_edges");
    bool repair = CmdLine::get_arg_bool("repair");

    if(max_edges == 0) {
        max_edges = max_index_t();
    }

    try {
        Stopwatch W("Fill Holes");
        Mesh M;

        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M)) {
            return 1;
        }

        Logger::out("Holes") << "Input: " << M.vertices.nb() << " vertices, "
                                << M.facets.nb() << " facets" << std::endl;

        Logger::out("Holes") << "Filling holes (max_area=" << max_area 
                                << ", max_edges=" << max_edges << ")..." << std::endl;
        fill_holes(M, max_area, max_edges, repair);

        Logger::out("Holes") << "Result: " << M.vertices.nb() << " vertices, "
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