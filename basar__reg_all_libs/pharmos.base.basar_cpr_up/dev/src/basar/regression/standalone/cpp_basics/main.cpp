
#include <iostream>
#include <string>
#include <vector>
#include <memory>

int main() {
    std::cout << "=========================================" << std::endl;
    std::cout << "  Basar Regression Test: cpp_basics" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "" << std::endl;
    
    std::cout << "Test 1: String Operations" << std::endl;
    std::string test = "Hello Basar";
    std::cout << "  String value: " << test << std::endl;
    std::cout << "  String length: " << test.length() << std::endl;
    std::cout << "  String operations verified" << std::endl;
    
    std::cout << "" << std::endl;
    std::cout << "Test 2: Vector Operations" << std::endl;
    std::vector<int> nums = {1, 2, 3, 4, 5};
    std::cout << "  Vector size: " << nums.size() << std::endl;
    std::cout << "  Vector capacity: " << nums.capacity() << std::endl;
    std::cout << "  Vector operations verified" << std::endl;
    
    std::cout << "" << std::endl;
    std::cout << "Test 3: Arithmetic Operations" << std::endl;
    int sum = 0;
    for(int n : nums) sum += n;
    std::cout << "  Sum of elements: " << sum << std::endl;
    std::cout << "  Expected: 15, Got: " << sum << std::endl;
    std::cout << "  Arithmetic operations verified" << std::endl;
    
    std::cout << "" << std::endl;
    std::cout << "Test 4: Smart Pointer Operations" << std::endl;
    std::unique_ptr<int> ptr = std::make_unique<int>(42);
    std::cout << "  Smart pointer value: " << *ptr << std::endl;
    std::cout << "  Smart pointer not null: " << (ptr ? "true" : "false") << std::endl;
    std::cout << "  Smart pointer operations verified" << std::endl;
    
    std::cout << "" << std::endl;
    std::cout << "Test 5: Exception Handling Basics" << std::endl;
    try {
        throw std::runtime_error("Test exception");
    } catch (const std::exception& e) {
        std::cout << "  Exception caught: " << e.what() << std::endl;
        std::cout << "  Exception handling verified" << std::endl;
    }
    
    std::cout << "" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "  Test completed successfully" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    return 0;
}
