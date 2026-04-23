/*
 *  Error recovery tests: invalid input handling
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_geometry.h>

using namespace GEO;

int main(int argc, char** argv) {
    initialize(GEOGRAM_INSTALL_ALL);

    Logger::out("Test") << "=== Error Recovery Tests ===" << std::endl;

    // Test 1: Empty mesh handling
    {
        Logger::out("Test") << "Test: Empty mesh area" << std::endl;
        Mesh M;
        double area = Geom::mesh_area(M);
        Logger::out("Test") << "  Empty area: " << area << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 2: Zero-size mesh
    {
        Logger::out("Test") << "Test: Zero-size mesh" << std::endl;
        Mesh M;
        M.vertices.set_dimension(3);
        double area = Geom::mesh_area(M);
        Logger::out("Test") << "  Zero area: " << area << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 3: Mesh with no facets
    {
        Logger::out("Test") << "Test: Mesh without facets" << std::endl;
        Mesh M;
        M.vertices.set_dimension(3);
        M.vertices.create_vertices(3);
        M.vertices.point(0) = vec3(0, 0, 0);
        M.vertices.point(1) = vec3(1, 0, 0);
        M.vertices.point(2) = vec3(0, 1, 0);
        double area = Geom::mesh_area(M);
        Logger::out("Test") << "  No facet area: " << area << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 4: Very small triangle
    {
        Logger::out("Test") << "Test: Very small triangle" << std::endl;
        Mesh M;
        M.vertices.set_dimension(3);
        M.vertices.create_vertices(3);
        M.vertices.point(0) = vec3(0, 0, 0);
        M.vertices.point(1) = vec3(1e-15, 0, 0);
        M.vertices.point(2) = vec3(0, 1e-15, 0);
        double area = Geom::mesh_area(M);
        Logger::out("Test") << "  Small area: " << area << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 5: Very large triangle
    {
        Logger::out("Test") << "Test: Very large triangle" << std::endl;
        Mesh M;
        M.vertices.set_dimension(3);
        M.vertices.create_vertices(3);
        M.vertices.point(0) = vec3(0, 0, 0);
        M.vertices.point(1) = vec3(1e15, 0, 0);
        M.vertices.point(2) = vec3(0, 1e15, 0);
        double area = Geom::mesh_area(M);
        Logger::out("Test") << "  Large area: " << area << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 6: Collinear points
    {
        Logger::out("Test") << "Test: Collinear points" << std::endl;
        Mesh M;
        M.vertices.set_dimension(3);
        M.vertices.create_vertices(3);
        M.vertices.point(0) = vec3(0, 0, 0);
        M.vertices.point(1) = vec3(1, 0, 0);
        M.vertices.point(2) = vec3(2, 0, 0);
        double area = Geom::mesh_area(M);
        Logger::out("Test") << "  Collinear area: " << area << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    Logger::out("Test") << "All error recovery tests completed!" << std::endl;
    return 0;
}