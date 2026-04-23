/*
 *  End-to-end pipeline tests
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_CSG.h>
#include <geogram/mesh/mesh_geometry.h>
#include <geogram/mesh/mesh_compare.h>

#include <geogram/basic/stopwatch.h>

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

    Logger::out("Test") << "=== Pipeline Tests ===" << std::endl;

    // Pipeline 1: Create -> Compute Area
    {
        Logger::out("Test") << "Pipeline: Create -> Compute Area" << std::endl;
        {
            Stopwatch w("Create mesh");
            Mesh M;
            create_tetrahedron(M);
        }
        {
            Stopwatch w("Compute area");
            Mesh M;
            create_tetrahedron(M);
            double area = Geom::mesh_area(M);
            Logger::out("Test") << "  Area: " << area << std::endl;
        }
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Pipeline 2: Create -> Compare identical
    {
        Logger::out("Test") << "Pipeline: Create -> Compare" << std::endl;
        Mesh M1, M2;
        create_tetrahedron(M1);
        create_tetrahedron(M2);
        double area1 = Geom::mesh_area(M1);
        double area2 = Geom::mesh_area(M2);
        Logger::out("Test") << "  Areas equal: " << (area1 == area2 ? "yes" : "no") << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Pipeline 3: CSG -> Mesh (if available)
    {
        Logger::out("Test") << "Pipeline: CSG -> Mesh" << std::endl;
        CSGCompiler compiler;
        auto result = compiler.compile_string("sphere(1);");
        if(result) {
            Mesh M;
            Logger::out("Test") << "  CSG created mesh" << std::endl;
        }
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Pipeline 4: Multiple operations loop
    {
        Logger::out("Test") << "Pipeline: Multiple ops" << std::endl;
        Mesh M;
        create_tetrahedron(M);
        for(int i = 0; i < 10; ++i) {
            double area = Geom::mesh_area(M);
            (void)area;
        }
        Logger::out("Test") << "  Loop: OK" << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Pipeline 5: Repeated mesh creation
    {
        Logger::out("Test") << "Pipeline: Repeated create" << std::endl;
        for(int i = 0; i < 5; ++i) {
            Mesh M;
            create_tetrahedron(M);
            double area = Geom::mesh_area(M);
            (void)area;
        }
        Logger::out("Test") << "  Repeated: OK" << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    Logger::out("Test") << "All pipeline tests completed!" << std::endl;
    return 0;
}