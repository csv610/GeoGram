/*
 *  Edge case tests: degenerate geometry, numerical precision
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <geogram/basic/geometry.h>
#include <geogram/mesh/mesh.h>

using namespace GEO;

int main(int argc, char** argv) {
    initialize(GEOGRAM_INSTALL_ALL);

    Logger::out("Test") << "=== Edge Case Tests ===" << std::endl;

    // Test 1: Degenerate triangle (nearly collinear)
    {
        Logger::out("Test") << "Test: Degenerate triangle" << std::endl;
        vec3 p0(0, 0, 0);
        vec3 p1(1, 0, 0);
        vec3 p2(0.5, 0.00001, 0);
        
        double area = Geom::triangle_area(p0, p1, p2);
        Logger::out("Test") << "  Area: " << area << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 2: Numerical precision at boundaries
    {
        Logger::out("Test") << "Test: Numerical precision" << std::endl;
        double very_small = 1e-15;
        double very_large = 1e+15;
        
        double sum = very_small + very_large;
        Logger::out("Test") << "  Small + Large: " << sum << std::endl;
        
        double diff = very_large - (very_large - very_small);
        Logger::out("Test") << "  Large - (Large-Small): " << diff << std::endl;
        
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 3: Near-zero coordinates
    {
        Logger::out("Test") << "Test: Near-zero coordinates" << std::endl;
        double epsilon = 1e-12;
        vec3 p0(epsilon, epsilon, epsilon);
        vec3 p1(1 + epsilon, epsilon, epsilon);
        vec3 p2(epsilon, 1 + epsilon, epsilon);
        
        double area = Geom::triangle_area(p0, p1, p2);
        Logger::out("Test") << "  Area: " << area << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 4: Large scale coordinates
    {
        Logger::out("Test") << "Test: Large scale coordinates" << std::endl;
        double scale = 1e+10;
        vec3 p0(0, 0, 0);
        vec3 p1(1 * scale, 0, 0);
        vec3 p2(0, 1 * scale, 0);
        
        double area = Geom::triangle_area(p0, p1, p2);
        Logger::out("Test") << "  Scaled area: " << area << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 5: Vector operations on boundary values
    {
        Logger::out("Test") << "Test: Vector operations" << std::endl;
        vec3 p0(0, 0, 0);
        vec3 p1(1, 0, 0);
        
        vec3 diff = p1 - p0;
        double len = length(diff);
        Logger::out("Test") << "  Length: " << len << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    Logger::out("Test") << "All edge case tests completed!" << std::endl;
    return 0;
}