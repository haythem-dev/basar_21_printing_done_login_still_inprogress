
#include <iostream>
#include <memory>
#include <vector>

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: memory_test" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 1: Smart Pointer Usage
        std::cout << "Test 1: Smart Pointer Usage" << std::endl;
        std::unique_ptr<int> uptr = std::make_unique<int>(42);
        std::cout << "  unique_ptr created with value: " << *uptr << std::endl;
        
        std::shared_ptr<int> sptr = std::make_shared<int>(100);
        std::cout << "  shared_ptr created with value: " << *sptr << std::endl;
        std::cout << "  shared_ptr use count: " << sptr.use_count() << std::endl;
        
        // Test 2: Vector Memory Management
        std::cout << "" << std::endl;
        std::cout << "Test 2: Vector Memory Management" << std::endl;
        std::vector<int> vec;
        vec.reserve(1000);
        std::cout << "  Vector reserved capacity: " << vec.capacity() << std::endl;
        
        for (int i = 0; i < 100; ++i) {
            vec.push_back(i);
        }
        std::cout << "  Vector size after push: " << vec.size() << std::endl;
        
        // Test 3: Memory Allocation
        std::cout << "" << std::endl;
        std::cout << "Test 3: Memory Allocation" << std::endl;
        int* raw = new int[100];
        std::cout << "  Raw array allocated" << std::endl;
        delete[] raw;
        std::cout << "  Raw array deallocated" << std::endl;
        
        // Test 4: RAII Pattern
        std::cout << "" << std::endl;
        std::cout << "Test 4: RAII Pattern" << std::endl;
        {
            std::unique_ptr<std::vector<int>> scopedVec = 
                std::make_unique<std::vector<int>>(1000);
            std::cout << "  Scoped resource created" << std::endl;
        }
        std::cout << "  Scoped resource automatically freed" << std::endl;
        
        // Test 5: Move Semantics
        std::cout << "" << std::endl;
        std::cout << "Test 5: Move Semantics" << std::endl;
        std::vector<int> source{1, 2, 3, 4, 5};
        std::cout << "  Source vector size: " << source.size() << std::endl;
        std::vector<int> dest = std::move(source);
        std::cout << "  Dest vector size after move: " << dest.size() << std::endl;
        std::cout << "  Source vector size after move: " << source.size() << std::endl;
        
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
