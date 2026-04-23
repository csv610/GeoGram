#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_AABB.h>
#include <geogram/mesh/mesh_geometry.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("input", "", "Path to the input mesh file");
    CmdLine::declare_arg("origin", "0,0,0", "Ray origin (x,y,z)");
    CmdLine::declare_arg("direction", "0,0,-1", "Ray direction (x,y,z)");
    CmdLine::declare_arg("tmax", 1e10, "Maximum ray parameter");
    CmdLine::declare_arg("output", "", "Output file for intersection point");

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

    std::string origin_str = CmdLine::get_arg("origin");
    std::string direction_str = CmdLine::get_arg("direction");
    double tmax = CmdLine::get_arg_double("tmax");
    std::string output_file = CmdLine::get_arg("output");

    vec3 origin(0,0,0);
    vec3 direction(0,0,-1);

    {
        double x,y,z;
        char comma;
        std::istringstream(origin_str) >> x >> comma >> y >> comma >> z;
        origin = vec3(x,y,z);
    }
    {
        double x,y,z;
        char comma;
        std::istringstream(direction_str) >> x >> comma >> y >> comma >> z;
        direction = vec3(x,y,z);
    }

    if(direction.length() < 1e-10) {
        Logger::err("Ray") << "Direction cannot be zero vector" << std::endl;
        return 1;
    }

    try {
        Stopwatch W("Ray Tracing");
        Mesh M;

        Logger::out("I/O") << "Loading " << input_file << "..." << std::endl;
        if(!mesh_load(input_file, M)) {
            return 1;
        }

        Logger::out("Ray") << "Building AABB tree..." << std::endl;
        MeshFacetsAABB tree(M);

        Ray R(origin, direction);

        Logger::out("Ray") << "Origin: " << origin << std::endl;
        Logger::out("Ray") << "Direction: " << direction << std::endl;

        MeshFacetsAABB::Intersection I;
        I.t = tmax;

        bool found = tree.ray_nearest_intersection(R, I);

        if(found) {
            vec3 hit_point = I.p;
            Logger::out("Ray") << "Intersection found!" << std::endl;
            Logger::out("Ray") << "  Facet: " << I.f << std::endl;
            Logger::out("Ray") << "  t: " << I.t << std::endl;
            Logger::out("Ray") << "  Point: " << hit_point << std::endl;
            Logger::out("Ray") << "  Barycentric: (" << 1.0-I.u-I.v << ", " << I.u << ", " << I.v << ")" << std::endl;

            if(!output_file.empty()) {
                Mesh hit_mesh;
                hit_mesh.vertices.set_dimension(3);
                hit_mesh.vertices.create_vertex(hit_point);
                mesh_save(hit_mesh, output_file);
                Logger::out("I/O") << "Saved intersection to " << output_file << std::endl;
            }
        } else {
            Logger::out("Ray") << "No intersection found." << std::endl;
        }

        Logger::out("App") << "Done." << std::endl;
    }
    catch(const std::exception& e) {
        Logger::err("App") << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}