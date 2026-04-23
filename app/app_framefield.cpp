#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input mesh file");
    CmdLine::declare_arg("output", "field.meshb", "Path to the output mesh file");

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

    try {
        Stopwatch W("Frame Field");
        Mesh M;

        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M)) {
            return 1;
        }

        Logger::out("Field") << "Input: " << M.vertices.nb() << " vertices, "
                               << M.facets.nb() << " facets" << std::endl;

        if(M.facets.nb() == 0) {
            Logger::err("Field") << "Mesh has no facets." << std::endl;
            return 1;
        }

        Logger::out("Field") << "Computing tangent direction field..." << std::endl;

        Attribute<vec3> tangent(M.facets.attributes(), "tangent");

        double total_len = 0.0;
        for(index_t f = 0; f < M.facets.nb(); ++f) {
            index_t v1 = M.facets.vertex(f,0);
            index_t v2 = M.facets.vertex(f,1);
            vec3 p1 = M.vertices.point(v1);
            vec3 p2 = M.vertices.point(v2);
            tangent[f] = p2 - p1;
            total_len += tangent[f].length();
        }

        Logger::out("Field") << "Frame field computed (tangent attribute)" << std::endl;
        Logger::out("Field") << "Total tangent length: " << total_len << std::endl;

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