#include <iostream>
#include <string>
#include <stdexcept>
#include "../include/HashMap.h"

int tests_passed = 0;
int tests_total = 0;

void EXPECT_EQ(int expected, int actual, const std::string& test_name) {
    tests_total++;
    if (expected == actual) {
        std::cout << "[PASS] " << test_name << "\n";
        tests_passed++;
    } else {
        std::cout << "[FAIL] " << test_name << " (Expected: " << expected << ", Got: " << actual << ")\n";
    }
}

void EXPECT_TRUE(bool condition, const std::string& test_name) {
    tests_total++;
    if (condition) {
        std::cout << "[PASS] " << test_name << "\n";
        tests_passed++;
    } else {
        std::cout << "[FAIL] " << test_name << " (Expected True, Got False)\n";
    }
}

void test_initial_state() {
    std::cout << "\n--- Testing Initial State ---\n";
    HashMap<std::string, int> map;
    EXPECT_EQ(0, map.size(), "Initial size is 0");
    EXPECT_TRUE(map.isEmpty(), "Map is empty initially");
}

void test_put() {
    std::cout << "\n--- Testing Put ---\n";
    HashMap<std::string, int> map;
    
    map.put("Alice", 25);
    map.put("Bob", 30);
    EXPECT_EQ(2, map.size(), "Size is 2 after two unique puts");
    
    // Testing the bug!
    map.put("Alice", 26); // Updating Alice's age
    
    // Due to the bug, it just appends a new KeyValuePair, so size becomes 3!
    EXPECT_EQ(2, map.size(), "Size should still be 2 after updating an existing key");
}

int main() {
    std::cout << "Starting HashMap Tests...\n";
    
    // Run the tests
    test_initial_state();
    test_put();
    
    // Print Summary
    std::cout << "\n==============================\n";
    std::cout << "Tests Passed: " << tests_passed << " / " << tests_total << "\n";
    if (tests_passed == tests_total) {
        std::cout << "ALL TESTS PASSED!\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED!\n";
        return 1;
    }
}
