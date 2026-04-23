#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_tetrahedralize.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input surface mesh file");
    CmdLine::declare_arg("output", "tetra.meshb", "Path to the output tetrahedral mesh file");
    CmdLine::declare_arg("preprocess", true, "Preprocess mesh (fix defects)");
    CmdLine::declare_arg("refine", true, "Refine mesh (insert vertices for quality)");
    CmdLine::declare_arg("quality", 2.0, "Desired mesh quality (1.0=best, 2.0=balanced)");
    CmdLine::declare_arg("keep_regions", false, "Keep internal regions");
    CmdLine::declare_arg("merge_eps", 0.001, "Vertex merge epsilon (percent of bbox diagonal)");

    std::vector<std::string> filenames;
    if(!CmdLine::parse(argc, argv, filenames, "input_mesh")) {
        return 1;
    }

    std::string input_file = CmdLine::get_arg("input");
    if(input_file.empty() && !filenames.empty()) {
        input_file = filenames[0];
    }

    if(input_file.empty()) {
        Logger::err("App") << "Please provide an input surface mesh file." << std::endl;
        return 1;
    }

    std::string output_file = CmdLine::get_arg("output");
    bool preprocess = CmdLine::get_arg_bool("preprocess");
    bool refine = CmdLine::get_arg_bool("refine");
    double quality = CmdLine::get_arg_double("quality");
    bool keep_regions = CmdLine::get_arg_bool("keep_regions");
    double merge_eps = CmdLine::get_arg_double("merge_eps");

    try {
        Stopwatch W("Tetrahedralize");
        Mesh M;

        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M)) {
            return 1;
        }

        Logger::out("Tetra") << "Input: " << M.vertices.nb() << " vertices, "
                                << M.facets.nb() << " facets" << std::endl;

        MeshTetrahedralizeParameters params;
        params.preprocess = preprocess;
        params.refine = refine;
        params.refine_quality = quality;
        params.keep_regions = keep_regions;
        params.preprocess_merge_vertices_epsilon = merge_eps;
        params.verbose = true;

        Logger::out("Tetra") << "Preprocess: " << (preprocess ? "yes" : "no") << std::endl;
        Logger::out("Tetra") << "Refine: " << (refine ? "yes" : "no") << std::endl;
        Logger::out("Tetra") << "Quality: " << quality << std::endl;

        bool success = mesh_tetrahedralize(M, params);

        if(!success) {
            Logger::err("Tetra") << "Tetrahedralization failed." << std::endl;
            return 1;
        }

        Logger::out("Tetra") << "Result: " << M.vertices.nb() << " vertices, "
                                << M.cells.nb() << " tetrahedra" << std::endl;

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