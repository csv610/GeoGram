#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_repair.h>

/**
 * A C++ CLI application to repair a mesh.
 * It can merge colocated vertices, remove duplicated facets, triangulate,
 * and reorient facets.
 */
int main(int argc, char** argv) {
    using namespace GEO;

    // 1. Initialize Geogram
    initialize(GEOGRAM_INSTALL_ALL);

    // 2. Configure the command line
    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input mesh file");
    CmdLine::declare_arg("output", "repaired.meshb", "Path to the output mesh file");
    CmdLine::declare_arg("epsilon", 1e-6, "Tolerance for merging vertices");
    CmdLine::declare_arg("reorient", true, "Consistently reorient facets");
    CmdLine::declare_arg("colocate", true, "Merge colocated vertices");
    CmdLine::declare_arg("remove_dup", true, "Remove duplicated facets");
    CmdLine::declare_arg("triangulate", false, "Triangulate the mesh");

    std::vector<std::string> filenames;
    if(!CmdLine::parse(argc, argv, filenames, "input_mesh")) {
        return 1;
    }

    std::string input_file = CmdLine::get_arg("input");
    if(input_file.empty() && !filenames.empty()) {
        input_file = filenames[0];
    }
    
    if(input_file.empty()) {
        Logger::err("Repair") << "Please provide an input mesh." << std::endl;
        return 1;
    }

    std::string output_file = CmdLine::get_arg("output");
    double epsilon = CmdLine::get_arg_double("epsilon");

    try {
        Stopwatch W("Total repair time");
        Mesh M;

        // 3. Load mesh
        Logger::out("I/O") << "Loading mesh " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M)) return 1;

        // 4. Construct repair mode
        int mode = MESH_REPAIR_TOPOLOGY;
        if(CmdLine::get_arg_bool("colocate")) mode |= MESH_REPAIR_COLOCATE;
        if(CmdLine::get_arg_bool("remove_dup")) mode |= MESH_REPAIR_DUP_F;
        if(CmdLine::get_arg_bool("triangulate")) mode |= MESH_REPAIR_TRIANGULATE;

        // 5. Perform repair
        Logger::out("Repair") << "Repairing mesh (epsilon=" << epsilon << ")..." << std::endl;
        mesh_repair(M, MeshRepairMode(mode), epsilon);

        // 6. Optional reorientation
        if(CmdLine::get_arg_bool("reorient")) {
            Logger::out("Repair") << "Reorienting facets..." << std::endl;
            mesh_reorient(M);
        }

        Logger::out("Repair") << "Mesh now has " << M.vertices.nb() << " vertices and "
                              << M.facets.nb() << " facets." << std::endl;

        // 7. Save result
        Logger::out("I/O") << "Saving repaired mesh to " << output_file << "..." << std::endl;
        if(!mesh_save(M, output_file)) return 1;

        Logger::out("Repair") << "Done." << std::endl;
    }
    catch(const std::exception& e) {
        Logger::err("Repair") << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
