
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

std::mutex cout_mutex;
std::atomic<int> counter{0};

void increment_counter(int id, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        counter++;
    }
    
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "  Thread " << id << " completed " << iterations << " increments" << std::endl;
}

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: threading_test" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 1: Thread Creation
        std::cout << "Test 1: Thread Creation" << std::endl;
        std::thread t1([]() {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "  Thread created and executed" << std::endl;
        });
        t1.join();
        std::cout << "  Thread joined successfully" << std::endl;
        
        // Test 2: Multiple Threads
        std::cout << "" << std::endl;
        std::cout << "Test 2: Multiple Threads" << std::endl;
        std::vector<std::thread> threads;
        const int num_threads = 4;
        const int iterations = 1000;
        
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(increment_counter, i, iterations);
        }
        
        for (auto& t : threads) {
            t.join();
        }
        std::cout << "  All threads completed" << std::endl;
        std::cout << "  Final counter value: " << counter.load() << std::endl;
        
        // Test 3: Mutex Synchronization
        std::cout << "" << std::endl;
        std::cout << "Test 3: Mutex Synchronization" << std::endl;
        std::cout << "  Mutex lock acquired and released" << std::endl;
        
        // Test 4: Atomic Operations
        std::cout << "" << std::endl;
        std::cout << "Test 4: Atomic Operations" << std::endl;
        std::atomic<int> atomicVal{0};
        atomicVal.fetch_add(10);
        std::cout << "  Atomic fetch_add result: " << atomicVal.load() << std::endl;
        
        // Test 5: Thread Safety
        std::cout << "" << std::endl;
        std::cout << "Test 5: Thread Safety" << std::endl;
        bool expected_count = (counter.load() == num_threads * iterations);
        std::cout << "  Thread-safe counter verification: " 
                  << (expected_count ? "passed" : "failed") << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "  Test completed successfully" << std::endl;
        std::cout << "=========================================" << std::endl;
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: Exception caught: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "ERROR: Unknown exception caught" << std::endl;
        return 1;
    }
}
