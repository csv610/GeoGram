#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_CSG.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input CSG file");
    CmdLine::declare_arg("output", "output.meshb", "Path to the output mesh file");
    CmdLine::declare_arg("source", "", "CSG source code as string");
    CmdLine::declare_arg("fn", 0.0, "Number of fragments (0=auto)");
    CmdLine::declare_arg("fs", 2.0, "Minimum fragment size");
    CmdLine::declare_arg("fa", 12.0, "Minimum fragment angle (degrees)");
    CmdLine::declare_arg("verbose", false, "Verbose output");
    CmdLine::declare_arg("detailed_verbose", false, "Detailed verbose output");

    std::vector<std::string> filenames;
    if(!CmdLine::parse(argc, argv, filenames, "input_mesh")) {
        return 1;
    }

    std::string input_file = CmdLine::get_arg("input");
    if(input_file.empty() && !filenames.empty()) {
        input_file = filenames[0];
    }

    std::string source = CmdLine::get_arg("source");
    std::string output_file = CmdLine::get_arg("output");

    if(input_file.empty() && source.empty()) {
        Logger::err("App") << "Please provide either -input <file> or -source <code>" << std::endl;
        return 1;
    }

    double fn = CmdLine::get_arg_double("fn");
    double fs = CmdLine::get_arg_double("fs");
    double fa = CmdLine::get_arg_double("fa");
    bool verbose = CmdLine::get_arg_bool("verbose");
    bool detailed_verbose = CmdLine::get_arg_bool("detailed_verbose");

    try {
        Stopwatch W("CSG Compilation");
        CSGCompiler compiler;
        std::shared_ptr<Mesh> result;

        if(verbose) compiler.set_verbose(true);
        if(detailed_verbose) compiler.set_detailed_verbose(true);

        compiler.builder().set_fn(fn);
        compiler.builder().set_fs(fs);
        compiler.builder().set_fa(fa);

        if(!input_file.empty()) {
            Logger::out("CSG") << "Compiling " << input_file << "..." << std::endl;
            result = compiler.compile_file(input_file);
        } else {
            Logger::out("CSG") << "Compiling source..." << std::endl;
            result = compiler.compile_string(source);
        }

        if(!result) {
            Logger::err("CSG") << "Compilation failed" << std::endl;
            return 1;
        }

        Mesh& M = *result;
        Logger::out("CSG") << "Result: " << M.vertices.nb() << " vertices, "
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