/*
 *  Test suite for mesh CSG operations
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_CSG.h>
#include <geogram/mesh/mesh_io.h>

using namespace GEO;

int main(int argc, char** argv) {
    initialize(GEOGRAM_INSTALL_ALL);

    Logger::out("Test") << "=== Testing Mesh CSG ===" << std::endl;

    // Test CSGCompiler
    {
        Logger::out("Test") << "Test CSGCompiler compile_string" << std::endl;
        CSGCompiler compiler;
        std::shared_ptr<Mesh> result = compiler.compile_string("cube(10);");
        if(!result) {
            Logger::err("Test") << "Failed: CSGCompiler returned null" << std::endl;
            return 1;
        }
        if(result->vertices.nb() == 0) {
            Logger::err("Test") << "Failed: no vertices in CSG result" << std::endl;
            return 1;
        }
        Logger::out("Test") << "  Result: " << result->vertices.nb() << " vertices, "
                         << result->facets.nb() << " facets" << std::endl;
    }

    // Test union operation
    {
        Logger::out("Test") << "Test CSG union" << std::endl;
        CSGCompiler compiler;
        std::shared_ptr<Mesh> result = compiler.compile_string("cube(10); sphere(5);");
        if(!result || result->facets.nb() == 0) {
            Logger::err("Test") << "Failed: union operation" << std::endl;
            return 1;
        }
        Logger::out("Test") << "  Union facets: " << result->facets.nb() << std::endl;
    }

    // Test cylinder
    {
        Logger::out("Test") << "Test CSG cylinder" << std::endl;
        CSGCompiler compiler;
        std::shared_ptr<Mesh> result = compiler.compile_string("cylinder(r=5, h=10);");
        if(!result || result->facets.nb() == 0) {
            Logger::err("Test") << "Failed: cylinder primitive" << std::endl;
            return 1;
        }
        Logger::out("Test") << "  Cylinder facets: " << result->facets.nb() << std::endl;
    }

    Logger::out("Test") << "All CSG tests passed!" << std::endl;
    return 0;
}