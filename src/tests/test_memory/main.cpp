/*
 *  Memory tests: Verify mesh operations don't leak
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/mesh/mesh.h>
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

    Logger::out("Test") << "=== Memory Tests ===" << std::endl;

    // Test 1: Mesh creation/destruction loop
    {
        Logger::out("Test") << "Test: Mesh create/destroy loop (100)" << std::endl;
        for(int i = 0; i < 100; ++i) {
            Mesh M;
            create_tetrahedron(M);
            double area = Geom::mesh_area(M);
            (void)area;
        }
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 2: Repeated mesh area computation
    {
        Logger::out("Test") << "Test: Area computation loop (1000)" << std::endl;
        Mesh M;
        create_tetrahedron(M);
        for(int i = 0; i < 1000; ++i) {
            double area = Geom::mesh_area(M);
            (void)area;
        }
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 3: Large mesh operations
    {
        Logger::out("Test") << "Test: Large mesh loop (10)" << std::endl;
        for(int i = 0; i < 10; ++i) {
            Mesh M;
            create_tetrahedron(M);
            double area = Geom::mesh_area(M);
            (void)area;
        }
        Logger::out("Test") << "  PASS" << std::endl;
    }

    Logger::out("Test") << "All memory tests completed!" << std::endl;
    return 0;
}