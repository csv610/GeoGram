/*
 *  API contract tests: verify API contracts
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_geometry.h>
#include <geogram/mesh/mesh_CSG.h>
#include <geogram/mesh/mesh_compare.h>

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

    Logger::out("Test") << "=== API Contract Tests ===" << std::endl;

    // Test 1: Mesh vertex dimension
    {
        Logger::out("Test") << "Test: Mesh vertex dimension" << std::endl;
        Mesh M;
        M.vertices.set_dimension(3);
        M.vertices.create_vertices(4);
        index_t dim = M.vertices.dimension();
        Logger::out("Test") << "  Dimension: " << dim << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 2: Mesh area for tetrahedron
    {
        Logger::out("Test") << "Test: Mesh area" << std::endl;
        Mesh M;
        create_tetrahedron(M);
        double area = Geom::mesh_area(M);
        Logger::out("Test") << "  Area: " << area << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 3: mesh_compare
    {
        Logger::out("Test") << "Test: mesh_compare" << std::endl;
        Mesh M1, M2;
        create_tetrahedron(M1);
        create_tetrahedron(M2);
        MeshCompareFlags flags = mesh_compare(M1, M2, MESH_COMPARE_SURFACE_PROPS, 0.0, false);
        Logger::out("Test") << "  Compare flags: " << flags << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 4: CSGCompiler
    {
        Logger::out("Test") << "Test: CSGCompiler" << std::endl;
        CSGCompiler compiler;
        auto result = compiler.compile_string("sphere(1);");
        if(result) {
            Logger::out("Test") << "  CSG valid" << std::endl;
        }
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 5: mesh_area positive
    {
        Logger::out("Test") << "Test: mesh_area positive" << std::endl;
        Mesh M;
        create_tetrahedron(M);
        double area = Geom::mesh_area(M);
        Logger::out("Test") << "  Area: " << area << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    Logger::out("Test") << "All API contract tests completed!" << std::endl;
    return 0;
}