#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_partition.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input mesh file");
    CmdLine::declare_arg("output", "output.meshb", "Path to the output mesh file");
    CmdLine::declare_arg("nb_parts", 4, "Number of parts to create");
    CmdLine::declare_arg("mode", "hilbert", "Partition mode (hilbert, connected)");

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
    index_t nb_parts = CmdLine::get_arg_uint("nb_parts");
    std::string mode_str = CmdLine::get_arg("mode");

    MeshPartitionMode mode = MESH_PARTITION_HILBERT;
    if(mode_str == "connected") mode = MESH_PARTITION_CONNECTED_COMPONENTS;
    else if(mode_str != "hilbert") {
        Logger::err("App") << "Unknown mode: " << mode_str << std::endl;
        return 1;
    }

    try {
        Stopwatch W("Mesh Partition");
        Mesh M;

        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M)) {
            return 1;
        }

        Logger::out("Partition") << "Input: " << M.vertices.nb() << " vertices, "
                                << M.facets.nb() << " facets" << std::endl;

        vector<index_t> facet_ptr;
        mesh_partition(M, mode, facet_ptr, nb_parts);

        index_t actual_nb_parts = (mode == MESH_PARTITION_CONNECTED_COMPONENTS) 
            ? facet_ptr.size() - 1 
            : nb_parts;

        Logger::out("Partition") << "Partitioned into " << actual_nb_parts << " parts" << std::endl;
        for(index_t p = 0; p < actual_nb_parts; ++p) {
            index_t start = facet_ptr[p];
            index_t end = facet_ptr[p+1];
            Logger::out("Partition") << "  Part " << p << ": " << (end - start) << " facets" << std::endl;
        }

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