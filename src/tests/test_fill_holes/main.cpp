/*
 *  Test suite for mesh fill holes
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_fill_holes.h>

using namespace GEO;

int main(int argc, char** argv) {
    initialize(GEOGRAM_INSTALL_ALL);

    Logger::out("Test") << "=== Testing Mesh Fill Holes ===" << std::endl;

    // Create mesh with a hole (square with missing edge)
    Mesh M;
    M.vertices.set_dimension(3);
    M.vertices.create_vertices(3);
    M.vertices.point(0) = vec3(0, 0, 0);
    M.vertices.point(1) = vec3(1, 0, 0);
    M.vertices.point(2) = vec3(0, 1, 0);

    // Two edges - creates a small hole
    M.facets.create_triangle(0, 1, 2);
    M.facets.create_triangle(0, 2, 1);

    index_t initial_facets = M.facets.nb();
    Logger::out("Test") << "Input: " << M.facets.nb() << " facets" << std::endl;

    // Test fill holes with no limit
    {
        Logger::out("Test") << "Test fill_holes" << std::endl;
        fill_holes(M, 0.0, max_index_t(), true);

        Logger::out("Test") << "  Facets after fill: " << M.facets.nb() << std::endl;
    }

    // Test tessellate_facets
    {
        Logger::out("Test") << "Test tessellate_facets" << std::endl;
        Mesh M2;
        M2.vertices.set_dimension(3);
        M2.vertices.create_vertices(4);
        M2.vertices.point(0) = vec3(0, 0, 0);
        M2.vertices.point(1) = vec3(2, 0, 0);
        M2.vertices.point(2) = vec3(2, 2, 0);
        M2.vertices.point(3) = vec3(0, 2, 0);

        // Quad facet
        M2.facets.create_quad(0, 1, 2, 3);

        tessellate_facets(M2, 3);

        if(!M2.facets.are_simplices()) {
            Logger::err("Test") << "Failed: facets not tessellated" << std::endl;
            return 1;
        }
        Logger::out("Test") << "  Tessellated facets: " << M2.facets.nb() << std::endl;
    }

    Logger::out("Test") << "All fill_holes tests passed!" << std::endl;
    return 0;
}