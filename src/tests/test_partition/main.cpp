/*
 *  Test suite for mesh partitioning
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_partition.h>

using namespace GEO;

int main(int argc, char** argv) {
    initialize(GEOGRAM_INSTALL_ALL);

    Logger::out("Test") << "=== Testing Mesh Partitioning ===" << std::endl;

    // Create mesh with two separate triangles
    Mesh M;
    M.vertices.set_dimension(3);
    // First triangle
    M.vertices.create_vertices(3);
    M.vertices.point(0) = vec3(0, 0, 0);
    M.vertices.point(1) = vec3(1, 0, 0);
    M.vertices.point(2) = vec3(0, 1, 0);
    M.facets.create_triangle(0, 1, 2);

    // Second triangle (separate)
    index_t base = 3;
    M.vertices.create_vertices(3);
    M.vertices.point(base+0) = vec3(5, 0, 0);
    M.vertices.point(base+1) = vec3(6, 0, 0);
    M.vertices.point(base+2) = vec3(5, 1, 0);
    M.facets.create_triangle(base+0, base+1, base+2);

    Logger::out("Test") << "Input: " << M.vertices.nb() << " vertices, "
                       << M.facets.nb() << " facets" << std::endl;

    // Test Hilbert partitioning
    {
        Logger::out("Test") << "Test Hilbert partition" << std::endl;
        vector<index_t> facet_ptr;
        vector<index_t> tet_ptr;
        const index_t nb_parts = 2;

        mesh_partition(M, MESH_PARTITION_HILBERT, facet_ptr, tet_ptr, nb_parts);

        if(facet_ptr.size() != nb_parts + 1) {
            Logger::err("Test") << "Failed: incorrect number of parts" << std::endl;
            return 1;
        }

        // Verify all facets are accounted for
        index_t total_facets = 0;
        for(index_t p = 0; p < nb_parts; ++p) {
            total_facets += facet_ptr[p+1] - facet_ptr[p];
        }
        if(total_facets != M.facets.nb()) {
            Logger::err("Test") << "Failed: facet count mismatch" << std::endl;
            return 1;
        }
        Logger::out("Test") << "  Partitioned into " << nb_parts << " parts" << std::endl;
    }

    // Test partition ranges
    {
        Logger::out("Test") << "Test partition ranges" << std::endl;
        vector<index_t> facet_ptr;
        vector<index_t> tet_ptr;
        const index_t nb_parts = 2;

        mesh_partition(M, MESH_PARTITION_HILBERT, facet_ptr, tet_ptr, nb_parts);

        // Verify facet_ptr is sorted
        for(index_t p = 0; p < nb_parts; ++p) {
            if(facet_ptr[p] >= facet_ptr[p+1]) {
                Logger::err("Test") << "Failed: facet_ptr not sorted at " << p << std::endl;
                return 1;
            }
        }
        Logger::out("Test") << "  Partition ranges valid" << std::endl;
    }

    Logger::out("Test") << "All partition tests passed!" << std::endl;
    return 0;
}