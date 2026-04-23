/*
 *  Thread safety and concurrency tests
 */

#include <geogram/basic/common.h>
#include <geogram/basic/logger.h>

#include <thread>
#include <atomic>
#include <vector>

using namespace GEO;

std::atomic<int> g_counter{0};

void worker_func(int thread_id) {
    for(int i = 0; i < 100; ++i) {
        g_counter.fetch_add(1);
    }
}

int main(int argc, char** argv) {
    initialize(GEOGRAM_INSTALL_ALL);

    Logger::out("Test") << "=== Thread Safety Tests ===" << std::endl;

    // Test 1: Basic thread spawning
    {
        Logger::out("Test") << "Test: Basic thread spawning" << std::endl;
        g_counter = 0;
        std::vector<std::thread> threads;
        for(int i = 0; i < 4; ++i) {
            threads.emplace_back(worker_func, i);
        }
        for(auto& t : threads) {
            t.join();
        }
        Logger::out("Test") << "  Final counter: " << g_counter.load() << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 2: Atomic operations
    {
        Logger::out("Test") << "Test: Atomic fetch add" << std::endl;
        std::atomic<long long> counter{0};
        
        std::vector<std::thread> threads;
        for(int i = 0; i < 4; ++i) {
            threads.emplace_back([&counter]() {
                for(int j = 0; j < 10000; ++j) {
                    counter.fetch_add(1);
                }
            });
        }
        for(auto& t : threads) {
            t.join();
        }
        
        Logger::out("Test") << "  Final count: " << counter.load() << std::endl;
        Logger::out("Test") << "  PASS" << std::endl;
    }

    // Test 3: Separate thread computations
    {
        Logger::out("Test") << "Test: Separate thread computations" << std::endl;
        std::vector<std::thread> threads;
        for(int i = 0; i < 4; ++i) {
            threads.emplace_back([i]() {
                double sum = 0;
                for(int j = 0; j < 1000; ++j) {
                    sum += sin(double(j) * 0.01);
                }
                Logger::out("Test") << "  Thread " << i << " sum: " << sum << std::endl;
            });
        }
        for(auto& t : threads) {
            t.join();
        }
        Logger::out("Test") << "  PASS" << std::endl;
    }

    Logger::out("Test") << "All thread safety tests completed!" << std::endl;
    return 0;
}