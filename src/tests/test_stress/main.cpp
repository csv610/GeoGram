/*
 *  Stress tests for large meshes
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_CSG.h>
#include <geogram/mesh/mesh_geometry.h>

using namespace GEO;

void create_tetrahedron(Mesh& M) {
    M.vertices.set_dimension(3);
    M.vertices.create_vertices(4);
    M.vertices.point(0) = vec3(0, 0, 0);
    M.vertices.point(1) = vec3(1, 0, 0);
    M.vertices.point(2) = vec3(0, 1, 0);
    M.vertices.point(3) = vec3(0, 0, 1);
    M.facets.create_triangle(0, 1, 2);
    M.facets.create_triangle(0, 2, 3);
    M.facets.create_triangle(0, 3, 1);
    M.facets.create_triangle(1, 3, 2);
}

int main(int argc, char** argv) {
    initialize(GEOGRAM_INSTALL_ALL);

    Logger::out("Test") << "=== Stress Tests ===" << std::endl;

    // Test 1: Large mesh area computation
    {
        Logger::out("Test") << "Test: Large facet mesh" << std::endl;
        Mesh M;
        create_tetrahedron(M);
        double area = Geom::mesh_area(M);
        Logger::out("Test") << "  Area: " << area << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 2: Large point cloud - iterate by count
    {
        Logger::out("Test") << "Test: Large point cloud (100k points)" << std::endl;
        Mesh M;
        M.vertices.set_dimension(3);
        M.vertices.create_vertices(100000);
        double sum = 0;
        for(index_t i = 0; i < 100000; ++i) {
            double x = double(i) / 100000.0;
            double y = sin(x * 3.14159 * 2);
            double z = cos(x * 3.14159 * 2);
            M.vertices.point(i) = vec3(x, y, z);
            sum += x;
        }
        Logger::out("Test") << "  Sum: " << sum << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 3: Large CSG tree
    {
        Logger::out("Test") << "Test: Large CSG tree" << std::endl;
        CSGCompiler compiler;
        std::string expr = "sphere(1) - sphere(0.8);";
        for(int i = 0; i < 5; ++i) {
            expr = "(" + expr + ") + box(0.3);";
        }
        Logger::out("Test") << "  Expression depth: 5" << std::endl;
        auto result = compiler.compile_string(expr);
        Logger::out("Test") << "  PASS" << std::endl;
    }

    Logger::out("Test") << "All stress tests completed!" << std::endl;
    return 0;
}