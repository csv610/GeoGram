#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_geometry.h>

/**
 * A C++ CLI application using the Geogram library.
 * This example loads a mesh, computes its area, and saves it.
 */
int main(int argc, char** argv) {
    using namespace GEO;

    // 1. Initialize the Geogram library
    initialize(GEOGRAM_INSTALL_ALL);

    // 2. Configure the command line
    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    // Declare arguments for our CLI
    CmdLine::declare_arg("input", "", "Path to the input mesh file");
    CmdLine::declare_arg("output", "output.meshb", "Path to the output mesh file");

    // Parse the command line
    std::vector<std::string> filenames;
    if(!CmdLine::parse(argc, argv, filenames, "input_mesh")) {
        return 1;
    }

    // Support both 'input=file' and positional 'file' arguments
    std::string input_file = CmdLine::get_arg("input");
    if(input_file.empty() && !filenames.empty()) {
        input_file = filenames[0];
    }
    
    if(input_file.empty()) {
        Logger::err("App") << "Please provide an input mesh file." << std::endl;
        return 1;
    }

    std::string output_file = CmdLine::get_arg("output");

    try {
        Stopwatch W("Total time");
        Mesh M;

        // 3. Load the mesh
        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M)) {
            return 1;
        }

        // 4. Perform some C++ geometry processing
        if(M.facets.nb() > 0) {
            double area = Geom::mesh_area(M);
            Logger::out("App") << "Mesh has " << M.facets.nb() << " facets." << std::endl;
            Logger::out("App") << "Total surface area: " << area << std::endl;
        } else {
            Logger::warn("App") << "Mesh has no facets." << std::endl;
        }

        // 5. Save the mesh
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
