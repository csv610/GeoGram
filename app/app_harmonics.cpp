#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_manifold_harmonics.h>
#include <geogram/basic/attributes.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input mesh file");
    CmdLine::declare_arg("output", "output.meshb", "Path to the output mesh file");
    CmdLine::declare_arg("nb_eigens", 10, "Number of eigenvectors to compute");
    CmdLine::declare_arg("discretization", "FEM_P1", "Discretization (COMBINATORIAL, UNIFORM, FEM_P1, FEM_P1_LUMPED)");
    CmdLine::declare_arg("shift", 0.0, "Eigenvalue shift");
    CmdLine::declare_arg("print_spectrum", false, "Print eigenvalues");
    CmdLine::declare_arg("attr_name", "eigen", "Attribute name for eigenvectors");

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
    index_t nb_eigens = CmdLine::get_arg_uint("nb_eigens");
    std::string discretization_str = CmdLine::get_arg("discretization");
    double shift = CmdLine::get_arg_double("shift");
    bool print_spectrum = CmdLine::get_arg_bool("print_spectrum");
    std::string attr_name = CmdLine::get_arg("attr_name");

    LaplaceBeltramiDiscretization discretization = FEM_P1;
    if(discretization_str == "COMBINATORIAL") discretization = COMBINATORIAL;
    else if(discretization_str == "UNIFORM") discretization = UNIFORM;
    else if(discretization_str == "FEM_P1") discretization = FEM_P1;
    else if(discretization_str == "FEM_P1_LUMPED") discretization = FEM_P1_LUMPED;
    else {
        Logger::err("App") << "Unknown discretization: " << discretization_str << std::endl;
        return 1;
    }

    try {
        Stopwatch W("Manifold Harmonics");
        Mesh M;

        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M)) {
            return 1;
        }

        Logger::out("Harmonics") << "Input: " << M.vertices.nb() << " vertices, "
                               << M.facets.nb() << " facets" << std::endl;
        Logger::out("Harmonics") << "Computing " << nb_eigens << " eigenfunctions..." << std::endl;

        mesh_compute_manifold_harmonics(M, nb_eigens, discretization, attr_name, shift, print_spectrum);

        Logger::out("Harmonics") << "Eigenvectors stored in attribute: " << attr_name << std::endl;

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