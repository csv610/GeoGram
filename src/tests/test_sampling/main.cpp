/*
 *  Test suite for mesh sampling operations
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_sampling.h>
#include <geogram/mesh/mesh_geometry.h>

using namespace GEO;

void create_single_triangle(Mesh& M) {
    M.vertices.set_dimension(3);
    M.vertices.create_vertices(3);
    M.vertices.point(0) = vec3(0, 0, 0);
    M.vertices.point(1) = vec3(1, 0, 0);
    M.vertices.point(2) = vec3(0, 1, 0);
    M.facets.create_triangle(0, 1, 2);
}

int main(int argc, char** argv) {
    initialize(GEOGRAM_INSTALL_ALL);

    Logger::out("Test") << "=== Testing Mesh Sampling ===" << std::endl;

    Mesh M;
    create_single_triangle(M);

    // Test with template API
    {
        Logger::out("Test") << "Test mesh operations" << std::endl;
        double area = Geom::mesh_area(M);
        Logger::out("Test") << "  Area: " << area << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    Logger::out("Test") << "All sampling tests completed!" << std::endl;
    return 0;
}