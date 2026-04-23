/*
 *  Performance benchmarks for Geogram
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/stopwatch.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_CSG.h>
#include <geogram/mesh/mesh_geometry.h>

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

void benchmark_mesh_area(int iterations) {
    Mesh M;
    create_tetrahedron(M);
    Stopwatch W("mesh_area x" + std::to_string(iterations));
    for(int i = 0; i < iterations; ++i) {
        Geom::mesh_area(M);
    }
}

void benchmark_csg(const std::string& shape, int scale) {
    Stopwatch W("CSG " + shape);
    CSGCompiler compiler;
    std::stringstream ss;
    ss << shape << "(" << scale << ");";
    auto result = compiler.compile_string(ss.str());
    if(result) {
        Logger::out("Bench") << "  " << shape << " created" << std::endl;
    }
}

void benchmark_mesh_create(int count) {
    Stopwatch W("Mesh create x" + std::to_string(count));
    for(int i = 0; i < count; ++i) {
        Mesh M;
        create_tetrahedron(M);
    }
}

int main(int argc, char** argv) {
    initialize(GEOGRAM_INSTALL_ALL);

    Logger::out("Bench") << "=== Geogram Performance Benchmarks ===" << std::endl;

    Logger::out("Bench") << "CSG Benchmarks:" << std::endl;
    benchmark_csg("sphere", 5);
    benchmark_csg("sphere", 10);
    benchmark_csg("box", 5);

    Logger::out("Bench") << "Geometry Benchmarks:" << std::endl;
    benchmark_mesh_area(1000);
    benchmark_mesh_area(10000);

    Logger::out("Bench") << "Creation Benchmarks:" << std::endl;
    benchmark_mesh_create(100);
    benchmark_mesh_create(1000);

    Logger::out("Bench") << "All benchmarks completed" << std::endl;
    return 0;
}