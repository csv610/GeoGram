#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_sampling.h>

#include <vector>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input mesh file");
    CmdLine::declare_arg("output", "samples.meshb", "Path to the output point cloud");
    CmdLine::declare_arg("nb_points", 1000, "Number of sample points");
    CmdLine::declare_arg("mode", "surface", "Sampling mode (surface, volume)");

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
    index_t nb_points = CmdLine::get_arg_uint("nb_points");
    std::string mode = CmdLine::get_arg("mode");

    try {
        Stopwatch W("Mesh Sampling");
        Mesh M;

        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M)) {
            return 1;
        }

        Logger::out("Sample") << "Input: " << M.vertices.nb() << " vertices, "
                               << M.facets.nb() << " facets, "
                               << M.cells.nb() << " cells" << std::endl;
        Logger::out("Sample") << "Requested samples: " << nb_points << std::endl;

        std::vector<double> p(nb_points * 3);
        Attribute<double> vertex_weight;
        bool success = false;

        if(mode == "surface") {
            if(M.facets.nb() == 0) {
                Logger::err("Sample") << "Mesh has no facets." << std::endl;
                return 1;
            }
            Logger::out("Sample") << "Sampling surface..." << std::endl;
            success = mesh_generate_random_samples_on_surface<3>(M, p.data(), nb_points, vertex_weight);
        } else {
            if(M.cells.nb() == 0) {
                Logger::err("Sample") << "Mesh has no cells." << std::endl;
                return 1;
            }
            Logger::out("Sample") << "Sampling volume..." << std::endl;
            success = mesh_generate_random_samples_in_volume<3>(M, p.data(), nb_points, vertex_weight);
        }

        if(!success) {
            Logger::warn("Sample") << "Sampling issues detected." << std::endl;
        }

        Mesh out_mesh;
        out_mesh.vertices.set_dimension(3);
        for(index_t i = 0; i < nb_points; ++i) {
            vec3 point(p[i*3], p[i*3+1], p[i*3+2]);
            out_mesh.vertices.create_vertex(point);
        }

        Logger::out("Sample") << "Generated: " << nb_points << " samples" << std::endl;

        Logger::out("I/O") << "Saving " << output_file << "..." << std::endl;
        if(!mesh_save(out_mesh, output_file)) {
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