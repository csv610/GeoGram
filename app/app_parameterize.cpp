#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_repair.h>
#include <geogram/parameterization/mesh_atlas_maker.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input mesh file");
    CmdLine::declare_arg("output", "parameterized.obj", "Path to the output mesh file (OBJ recommended for UVs)");
    CmdLine::declare_arg("method", "abf", "Method: projection, lscm, spectral, abf");
    CmdLine::declare_arg("pack", "tetris", "Packer: none, tetris, xatlas");
    CmdLine::declare_arg("threshold", 45.0, "Hard angle threshold for chart boundaries (degrees)");

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
    std::string method_str = CmdLine::get_arg("method");
    std::string pack_str = CmdLine::get_arg("pack");
    double threshold = CmdLine::get_arg_double("threshold");

    ChartParameterizer param = PARAM_ABF;
    if(method_str == "projection") param = PARAM_PROJECTION;
    else if(method_str == "lscm") param = PARAM_LSCM;
    else if(method_str == "spectral") param = PARAM_SPECTRAL_LSCM;
    else if(method_str == "abf") param = PARAM_ABF;

    ChartPacker pack = PACK_TETRIS;
    if(pack_str == "none") pack = PACK_NONE;
    else if(pack_str == "tetris") pack = PACK_TETRIS;
    else if(pack_str == "xatlas") pack = PACK_XATLAS;

    try {
        Stopwatch W("Parameterization");
        Mesh M;

        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M)) return 1;

        // Parameterization requires a triangulated mesh
        if(!M.facets.are_simplices()) {
            Logger::out("App") << "Triangulating mesh..." << std::endl;
            M.facets.triangulate();
        }

        Logger::out("App") << "Computing atlas using " << method_str << " and " << pack_str << " packing..." << std::endl;
        mesh_make_atlas(M, threshold, param, pack, true);

        Logger::out("I/O") << "Saving " << output_file << "..." << std::endl;
        // OBJ format is good for saving UV coordinates (tex_coord attribute)
        if(!mesh_save(M, output_file)) return 1;

        Logger::out("App") << "Done." << std::endl;
    }
    catch(const std::exception& e) {
        Logger::err("App") << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
