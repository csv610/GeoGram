/*
 *  Test suite for mesh comparison
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_compare.h>

using namespace GEO;

int main(int argc, char** argv) {
    initialize(GEOGRAM_INSTALL_ALL);

    Logger::out("Test") << "=== Testing Mesh Comparison ===" << std::endl;

    // Create two identical meshes
    Mesh M1, M2;
    M1.vertices.set_dimension(3);
    M1.vertices.create_vertices(3);
    M1.vertices.point(0) = vec3(0, 0, 0);
    M1.vertices.point(1) = vec3(1, 0, 0);
    M1.vertices.point(2) = vec3(0, 1, 0);
    M1.facets.create_triangle(0, 1, 2);

    M2.vertices.set_dimension(3);
    M2.vertices.create_vertices(3);
    M2.vertices.point(0) = vec3(0, 0, 0);
    M2.vertices.point(1) = vec3(1, 0, 0);
    M2.vertices.point(2) = vec3(0, 1, 0);
    M2.facets.create_triangle(0, 1, 2);

    // Test identical mesh comparison
    {
        Logger::out("Test") << "Test identical mesh comparison" << std::endl;
        MeshCompareFlags status = mesh_compare(M1, M2, MESH_COMPARE_SURFACE_PROPS, 0.0, false);
        if(status != MESH_COMPARE_OK) {
            Logger::err("Test") << "Failed: identical meshes not equal" << std::endl;
            return 1;
        }
        Logger::out("Test") << "  Identical meshes: OK" << std::endl;
    }

    // Create meshes with different vertex count
    {
        Logger::out("Test") << "Test differing vertex count" << std::endl;
        Mesh M3;
        M3.vertices.set_dimension(3);
        M3.vertices.create_vertices(2);
        M3.vertices.point(0) = vec3(0, 0, 0);
        M3.vertices.point(1) = vec3(1, 0, 0);

        Mesh M4;
        M4.vertices.set_dimension(3);
        M4.vertices.create_vertices(3);
        M4.vertices.point(0) = vec3(0, 0, 0);
        M4.vertices.point(1) = vec3(1, 0, 0);
        M4.vertices.point(2) = vec3(0, 1, 0);

        MeshCompareFlags status = mesh_compare(M3, M4, MESH_COMPARE_NB_VERTICES, 0.0, false);
        if(status == MESH_COMPARE_OK) {
            Logger::err("Test") << "Failed: should detect different vertex count" << std::endl;
            return 1;
        }
        Logger::out("Test") << "  Different meshes detected: OK" << std::endl;
    }

    // Test area comparison
    {
        Logger::out("Test") << "Test area comparison" << std::endl;
        Mesh M5, M6;
        M5.vertices.set_dimension(3);
        M5.vertices.create_vertices(3);
        M5.vertices.point(0) = vec3(0, 0, 0);
        M5.vertices.point(1) = vec3(1, 0, 0);
        M5.vertices.point(2) = vec3(0, 1, 0);
        M5.facets.create_triangle(0, 1, 2);

        M6.vertices.set_dimension(3);
        M6.vertices.create_vertices(3);
        M6.vertices.point(0) = vec3(0, 0, 0);
        M6.vertices.point(1) = vec3(1, 0, 0);
        M6.vertices.point(2) = vec3(0, 1, 0);
        M6.facets.create_triangle(0, 1, 2);

        MeshCompareFlags status = mesh_compare(M5, M6, MESH_COMPARE_AREAS, 0.0, false);
        if(status != MESH_COMPARE_OK) {
            Logger::err("Test") << "Failed: identical area meshes" << std::endl;
            return 1;
        }
        Logger::out("Test") << "  Area comparison: OK" << std::endl;
    }

    Logger::out("Test") << "All compare tests passed!" << std::endl;
    return 0;
}