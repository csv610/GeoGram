#include <geogram/basic/common.h>
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_compare.h>
#include <geogram/mesh/mesh_geometry.h>
#include <geogram/mesh/mesh_AABB.h>

int main(int argc, char** argv) {
    using namespace GEO;
    initialize(GEOGRAM_INSTALL_ALL);

    CmdLine::import_arg_group("standard");
    CmdLine::import_arg_group("algo");

    CmdLine::declare_arg("meshA", "", "Path to the first mesh file");
    CmdLine::declare_arg("meshB", "", "Path to the second mesh file");
    CmdLine::declare_arg("mode", "surface", "Comparison mode (surface, volume, full, hausdorff)");
    CmdLine::declare_arg("tolerance", 0.0, "Relative tolerance for floating point comparison");
    CmdLine::declare_arg("verbose", true, "Display detailed comparison");

    std::vector<std::string> filenames;
    if(!CmdLine::parse(argc, argv, filenames, "meshA meshB")) {
        return 1;
    }

    std::string meshA_file = CmdLine::get_arg("meshA");
    std::string meshB_file = CmdLine::get_arg("meshB");
    if(meshA_file.empty() && filenames.size() > 0) meshA_file = filenames[0];
    if(meshB_file.empty() && filenames.size() > 1) meshB_file = filenames[1];

    if(meshA_file.empty() || meshB_file.empty()) {
        Logger::err("App") << "Please provide two mesh files to compare." << std::endl;
        return 1;
    }

    std::string mode = CmdLine::get_arg("mode");
    double tolerance = CmdLine::get_arg_double("tolerance");
    bool verbose = CmdLine::get_arg_bool("verbose");

    try {
        Stopwatch W("Mesh Compare");
        Mesh A, B;

        Logger::out("I/O") << "Loading mesh A: " << meshA_file << "..." << std::endl;
        if(!mesh_load(meshA_file, A)) return 1;

        Logger::out("I/O") << "Loading mesh B: " << meshB_file << "..." << std::endl;
        if(!mesh_load(meshB_file, B)) return 1;

        Logger::out("Compare") << "Mesh A: " << A.vertices.nb() << " vertices, "
                             << A.facets.nb() << " facets" << std::endl;
        Logger::out("Compare") << "Mesh B: " << B.vertices.nb() << " vertices, "
                             << B.facets.nb() << " facets" << std::endl;

        if(mode == "hausdorff") {
            Logger::out("Compare") << "Computing Hausdorff distance..." << std::endl;

            MeshFacetsAABB treeA(A);
            MeshFacetsAABB treeB(B);

            double max_dist = 0.0;
            double sum_dist = 0.0;
            index_t count = 0;

            for(index_t i = 0; i < B.vertices.nb(); ++i) {
                vec3 p = B.vertices.point(i);
                MeshFacetsAABB::Intersection I;
                I.t = 1e10;
                if(treeA.ray_nearest_intersection(Ray(p, vec3(0,0,1)), I)) {
                    double d = sqrt((p - I.p).length2());
                    max_dist = std::max(max_dist, d);
                    sum_dist += d;
                    count++;
                }
            }
            double hausdorff_AB = (count > 0) ? sum_dist / count : 0.0;

            max_dist = 0.0;
            sum_dist = 0.0;
            count = 0;

            for(index_t i = 0; i < A.vertices.nb(); ++i) {
                vec3 p = A.vertices.point(i);
                MeshFacetsAABB::Intersection I;
                I.t = 1e10;
                if(treeB.ray_nearest_intersection(Ray(p, vec3(0,0,1)), I)) {
                    double d = sqrt((p - I.p).length2());
                    max_dist = std::max(max_dist, d);
                    sum_dist += d;
                    count++;
                }
            }
            double hausdorff_BA = (count > 0) ? sum_dist / count : 0.0;

            Logger::out("Compare") << "Hausdorff A->B (mean): " << hausdorff_AB << std::endl;
            Logger::out("Compare") << "Hausdorff B->A (mean): " << hausdorff_BA << std::endl;
            Logger::out("Compare") << "Hausdorff (max): " << std::max(hausdorff_AB, hausdorff_BA) << std::endl;

        } else {
            MeshCompareFlags flags = MESH_COMPARE_SURFACE_PROPS;
            if(mode == "volume") flags = MESH_COMPARE_VOLUME_PROPS;
            if(mode == "full") flags = MESH_COMPARE_ALL;

            MeshCompareFlags status = mesh_compare(A, B, flags, tolerance, verbose);

            if(status == MESH_COMPARE_OK) {
                Logger::out("Compare") << "Meshes are identical." << std::endl;
            } else {
                Logger::out("Compare") << "Meshes differ in:" << std::endl;
                if(status & MESH_COMPARE_DIMENSIONS) Logger::out("Compare") << "  - dimensions" << std::endl;
                if(status & MESH_COMPARE_NB_VERTICES) Logger::out("Compare") << "  - number of vertices" << std::endl;
                if(status & MESH_COMPARE_NB_FACETS) Logger::out("Compare") << "  - number of facets" << std::endl;
                if(status & MESH_COMPARE_AREAS) Logger::out("Compare") << "  - areas" << std::endl;
                if(status & MESH_COMPARE_TOPOLOGY) Logger::out("Compare") << "  - topology" << std::endl;
                if(status & MESH_COMPARE_NB_TETS) Logger::out("Compare") << "  - number of tetrahedra" << std::endl;
            }
        }

        Logger::out("App") << "Done." << std::endl;
    }
    catch(const std::exception& e) {
        Logger::err("App") << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}