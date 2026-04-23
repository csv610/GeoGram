#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/parameterization/mesh_segmentation.h>

/**
 * A C++ CLI application to segment a mesh into several parts.
 * It uses Variational Shape Approximation (VSA) or Spectral methods.
 */
int main(int argc, char** argv) {
    using namespace GEO;

    // 1. Initialize Geogram
    initialize(GEOGRAM_INSTALL_ALL);

    // 2. Configure the command line
    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input mesh file");
    CmdLine::declare_arg("output", "segmented.meshb", "Path to the output mesh file (with 'chart' attribute)");
    CmdLine::declare_arg("nb_parts", 10, "Target number of segments");
    CmdLine::declare_arg("method", "vsa", "Method: vsa, spectral, inertia");

    std::vector<std::string> filenames;
    if(!CmdLine::parse(argc, argv, filenames, "input_mesh")) {
        return 1;
    }

    std::string input_file = CmdLine::get_arg("input");
    if(input_file.empty() && !filenames.empty()) {
        input_file = filenames[0];
    }
    
    if(input_file.empty()) {
        Logger::err("Segment") << "Please provide an input mesh." << std::endl;
        return 1;
    }

    std::string output_file = CmdLine::get_arg("output");
    index_t nb_parts = CmdLine::get_arg_uint("nb_parts");
    std::string method_str = CmdLine::get_arg("method");

    MeshSegmenter segmenter = SEGMENT_GEOMETRIC_VSA_L2;
    if(method_str == "vsa") segmenter = SEGMENT_GEOMETRIC_VSA_L2;
    else if(method_str == "spectral") segmenter = SEGMENT_SPECTRAL_20;
    else if(method_str == "inertia") segmenter = SEGMENT_INERTIA_AXIS;

    try {
        Stopwatch W("Total segmentation time");
        Mesh M;

        // 3. Load mesh
        Logger::out("I/O") << "Loading mesh " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M)) return 1;

        // Segmentation requires a triangulated mesh
        if(!M.facets.are_simplices()) {
            Logger::out("App") << "Triangulating mesh..." << std::endl;
            M.facets.triangulate();
        }

        // 4. Compute segmentation
        Logger::out("Segment") << "Segmenting mesh into " << nb_parts << " parts using " << method_str << "..." << std::endl;
        
        index_t nb_charts = mesh_segment(M, segmenter, nb_parts, true);

        Logger::out("Segment") << "Generated " << nb_charts << " segments (stored in 'chart' facet attribute)." << std::endl;

        // 5. Save result
        Logger::out("I/O") << "Saving segmented mesh to " << output_file << "..." << std::endl;
        if(!mesh_save(M, output_file)) return 1;

        Logger::out("Segment") << "Done." << std::endl;
    }
    catch(const std::exception& e) {
        Logger::err("Segment") << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
