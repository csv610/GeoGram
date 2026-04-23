/*
 *  Test suite for mesh tetrahedralization
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_tetrahedralize.h>
#include <geogram/mesh/mesh_geometry.h>

using namespace GEO;

int main(int argc, char** argv) {
    initialize(GEOGRAM_INSTALL_ALL);

    Logger::out("Test") << "=== Testing Mesh Tetrahedralization ===" << std::endl;

    // Create closed surface mesh (tetrahedron)
    Mesh M;
    M.vertices.set_dimension(3);
    M.vertices.create_vertices(4);
    M.vertices.point(0) = vec3(0, 0, 0);
    M.vertices.point(1) = vec3(1, 0, 0);
    M.vertices.point(2) = vec3(0, 1, 0);
    M.vertices.point(3) = vec3(0, 0, 1);

    // Create outer facets (4 triangles)
    M.facets.create_triangle(0, 1, 2);  // bottom
    M.facets.create_triangle(0, 2, 3);  // front
    M.facets.create_triangle(0, 3, 1);  // left
    M.facets.create_triangle(1, 3, 2);  // right

    Logger::out("Test") << "Input: " << M.vertices.nb() << " vertices, "
                       << M.facets.nb() << " facets" << std::endl;

    // Test tetrahedralization
    {
        Logger::out("Test") << "Test tetrahedralize" << std::endl;
        MeshTetrahedralizeParameters params;
        params.preprocess = false;
        params.refine = false;
        params.verbose = false;

        bool success = mesh_tetrahedralize(M, params);
        if(!success) {
            Logger::err("Test") << "Failed: tetrahedralize returned false" << std::endl;
            return 1;
        }

        if(M.cells.nb() == 0) {
            Logger::err("Test") << "Failed: no cells created" << std::endl;
            return 1;
        }

        Logger::out("Test") << "  Tetrahedra: " << M.cells.nb() << std::endl;
    }

    // Test volume computation
    {
        Logger::out("Test") << "Test enclosed volume" << std::endl;
        double volume = Geom::mesh_enclosed_volume(M);
        if(volume <= 0) {
            Logger::err("Test") << "Failed: invalid enclosed volume" << std::endl;
            return 1;
        }
        // Tetrahedron volume = 1/6 for unit tetrahedron
        Logger::out("Test") << "  Enclosed volume: " << volume << std::endl;
    }

    Logger::out("Test") << "All tetrahedralize tests passed!" << std::endl;
    return 0;
}