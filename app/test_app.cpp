#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>
#include <iostream>

/**
 * A simple C++ test for our application logic.
 * In a real-world scenario, you would link against your app's library
 * and test its functions here.
 */
int main() {
    GEO::initialize(GEO::GEOGRAM_INSTALL_ALL);

    GEO::Logger::out("Test") << "Running C++ App Test..." << std::endl;

    // Example assertion
    bool condition = true; 
    
    if(condition) {
        GEO::Logger::out("Test") << "Test passed!" << std::endl;
        return 0;
    } else {
        GEO::Logger::err("Test") << "Test failed!" << std::endl;
        return 1;
    }
}
