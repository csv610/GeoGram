#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_surface_intersection.h>
#include <geogram/mesh/mesh_repair.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("meshA", "", "Path to the first mesh file");
    CmdLine::declare_arg("meshB", "", "Path to the second mesh file");
    CmdLine::declare_arg("output", "result.meshb", "Path to the output mesh file");
    CmdLine::declare_arg("op", "union", "Operation: union, intersection, difference");

    std::vector<std::string> filenames;
    if(!CmdLine::parse(argc, argv, filenames, "meshA meshB")) {
        return 1;
    }

    std::string meshA_file = CmdLine::get_arg("meshA");
    std::string meshB_file = CmdLine::get_arg("meshB");
    if(meshA_file.empty() && filenames.size() > 0) meshA_file = filenames[0];
    if(meshB_file.empty() && filenames.size() > 1) meshB_file = filenames[1];
    
    if(meshA_file.empty() || meshB_file.empty()) {
        Logger::err("App") << "Please provide two input mesh files." << std::endl;
        return 1;
    }

    std::string output_file = CmdLine::get_arg("output");
    std::string op = CmdLine::get_arg("op");

    try {
        Stopwatch W("Boolean Operations");
        Mesh A, B, result;

        Logger::out("I/O") << "Loading " << meshA_file << "..." << std::endl;
        if(!mesh_load(meshA_file, A)) return 1;

        Logger::out("I/O") << "Loading " << meshB_file << "..." << std::endl;
        if(!mesh_load(meshB_file, B)) return 1;

        Logger::out("Boolean") << "Operation: " << op << std::endl;

        MeshBooleanOperationFlags flags = MESH_BOOL_OPS_DEFAULT;
        
        if (op == "union") {
            mesh_union(result, A, B, flags);
        } else if (op == "intersection") {
            mesh_intersection(result, A, B, flags);
        } else if (op == "difference") {
            mesh_difference(result, A, B, flags);
        } else {
            Logger::err("Boolean") << "Unknown operation: " << op << std::endl;
            return 1;
        }

        Logger::out("I/O") << "Saving " << output_file << "..." << std::endl;
        if(!mesh_save(result, output_file)) return 1;
    }
    catch(const std::exception& e) {
        Logger::err("App") << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
