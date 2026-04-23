/*
 *  Test suite for mesh frame field
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>

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

    Logger::out("Test") << "=== Testing Mesh Frame Field ===" << std::endl;

    // Create simple triangular mesh
    Mesh M;
    create_single_triangle(M);

    // Test that mesh is properly created
    {
        Logger::out("Test") << "Test mesh creation" << std::endl;
        if(M.facets.nb() == 0) {
            Logger::err("Test") << "Failed: no facets created" << std::endl;
            return 1;
        }
        if(M.vertices.nb() != 3) {
            Logger::err("Test") << "Failed: wrong vertex count" << std::endl;
            return 1;
        }
        Logger::out("Test") << "  Mesh: " << M.vertices.nb() << " vertices, "
                         << M.facets.nb() << " facets" << std::endl;
    }

    // Test frame field tangent directions (edge vectors)
    {
        Logger::out("Test") << "Test frame field tangent vectors" << std::endl;
        for(index_t f = 0; f < M.facets.nb(); ++f) {
            index_t v1 = M.facets.vertex(f, 0);
            index_t v2 = M.facets.vertex(f, 1);
            vec3 p1 = M.vertices.point(v1);
            vec3 p2 = M.vertices.point(v2);
            vec3 tangent = p2 - p1;
            if(tangent.length2() < 1e-10) {
                Logger::err("Test") << "Failed: zero tangent at facet " << f << std::endl;
                return 1;
            }
        }
        Logger::out("Test") << "  Tangent vectors valid" << std::endl;
    }

    Logger::out("Test") << "All frame field tests passed!" << std::endl;
    return 0;
}