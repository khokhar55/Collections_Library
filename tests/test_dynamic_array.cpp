#include <iostream>
#include <string>
#include "../include/DynamicArray.h"

// --- Custom Testing Framework ---
// We use simple global counters to track test success without external libraries.
int testsPassed = 0;
int testsFailed = 0;
int testCaseCount = 0;

// Helper to check if two values are equal
template <typename T>
void EXPECT_EQ(const T& expected, const T& actual, const std::string& testName) {
    testCaseCount++;
    if (expected == actual) {
        std::cout << "[PASS] " << testName << "\n";
        testsPassed++;
    } else {
        std::cout << "[FAIL] " << testName << " (Expected: " << expected << ", Got: " << actual << ")\n";
        testsFailed++;
    }
}

// Helper to check boolean conditions
void EXPECT_TRUE(bool condition, const std::string& testName) {
    testCaseCount++;
    if (condition) {
        std::cout << "[PASS] " << testName << "\n";
        testsPassed++;
    } else {
        std::cout << "[FAIL] " << testName << " (Expected True, Got False)\n";
        testsFailed++;
    }
}

// --- Test Cases ---

void test_initial_state() {
    std::cout << "\n--- Testing Initial State ---\n";
    
    // Create an array of integers
    DynamicArray<int> arr;
    
    // Check 1: A new array should have exactly 0 elements
    EXPECT_EQ(0, arr.size(), "Initial size is 0");
    
    // Check 2: Our chosen default capacity is 4 slots
    EXPECT_EQ(4, arr.capacity(), "Initial capacity is 4");
    
    // Check 3: isEmpty() should return true
    EXPECT_TRUE(arr.isEmpty(), "Array is empty initially");
}

void test_custom_capacity() {
    std::cout << "\n--- Testing Custom Capacity Constructor ---\n";
    
    // Create an array with a specific capacity of 100
    DynamicArray<int> arr(100);
    
    // Size should still be 0
    EXPECT_EQ(0, arr.size(), "Custom capacity size is 0");
    
    // Capacity should be 100
    EXPECT_EQ(100, arr.capacity(), "Custom capacity is 100");
}

void test_element_access() {
    std::cout << "\n--- Testing Element Access ---\n";
    DynamicArray<int> arr; // Empty array (size = 0)
    
    // Attempting to get an element out of bounds should throw std::out_of_range
    bool exceptionThrown = false;
    try {
        arr.get(0);
    } catch (const std::out_of_range& e) {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown, "get() throws out_of_range on empty array");
}

void test_append_and_resize() {
    std::cout << "\n--- Testing Append & Resize ---\n";
    DynamicArray<int> arr; // Initial capacity is 4
    
    // Test appending a single item
    arr.append(10);
    EXPECT_EQ(1, arr.size(), "Size is 1 after first append");
    EXPECT_EQ(4, arr.capacity(), "Capacity is still 4");
    EXPECT_EQ(10, arr.get(0), "Element 0 is 10 (via get)");
    EXPECT_EQ(10, arr[0], "Element 0 is 10 (via operator[])");
    
    // Fill up to initial capacity (4)
    arr.append(20);
    arr.append(30);
    arr.append(40);
    EXPECT_EQ(4, arr.size(), "Size is 4 after 4 appends");
    EXPECT_EQ(4, arr.capacity(), "Capacity is still 4");
    EXPECT_EQ(40, arr.get(3), "Element 3 is 40");
    
    // Trigger the first resize (capacity should double to 8)
    arr.append(50);
    EXPECT_EQ(5, arr.size(), "Size is 5 after resize trigger");
    EXPECT_EQ(8, arr.capacity(), "Capacity doubled to 8");
    EXPECT_EQ(10, arr.get(0), "Element 0 is still 10 after resize");
    EXPECT_EQ(50, arr.get(4), "Element 4 is 50");
    
    // Trigger another resize by appending up to 9 items
    arr.append(60);
    arr.append(70);
    arr.append(80);
    arr.append(90); // 9th item -> resize to 16
    EXPECT_EQ(9, arr.size(), "Size is 9 after second resize trigger");
    EXPECT_EQ(16, arr.capacity(), "Capacity doubled to 16");
    EXPECT_EQ(90, arr.get(8), "Element 8 is 90");
    
    // Massive append to test large resizing
    DynamicArray<int> bigArr;
    for (int i = 0; i < 1000; ++i) {
        bigArr.append(i);
    }
    EXPECT_EQ(1000, bigArr.size(), "Large array size is 1000");
    EXPECT_EQ(1024, bigArr.capacity(), "Large array capacity is 1024 (2^10)");
    EXPECT_EQ(999, bigArr.get(999), "Last element is correct");
}

void test_insert() {
    std::cout << "\n--- Testing Insert ---\n";
    DynamicArray<int> arr;
    arr.append(10);
    arr.append(20);
    arr.append(30);
    
    // Insert at front
    arr.insert(0, 5); // Should become [5, 10, 20, 30]
    EXPECT_EQ(4, arr.size(), "Size is 4 after insert front");
    EXPECT_EQ(5, arr.get(0), "Element 0 is 5");
    EXPECT_EQ(10, arr.get(1), "Element 1 is 10");
    EXPECT_EQ(20, arr.get(2), "Element 2 is 20");
    EXPECT_EQ(30, arr.get(3), "Element 3 is 30");
    
    // Insert in middle
    arr.insert(2, 15); // Should become [5, 10, 15, 20, 30]
    EXPECT_EQ(5, arr.size(), "Size is 5 after insert middle");
    EXPECT_EQ(10, arr.get(1), "Element 1 is 10");
    EXPECT_EQ(15, arr.get(2), "Element 2 is 15");
    EXPECT_EQ(20, arr.get(3), "Element 3 is 20");
    
    // Insert at back (equivalent to append)
    arr.insert(5, 35); // Should become [5, 10, 15, 20, 30, 35]
    EXPECT_EQ(6, arr.size(), "Size is 6 after insert back");
    EXPECT_EQ(35, arr.get(5), "Element 5 is 35");
}

void test_remove_and_pop() {
    std::cout << "\n--- Testing Remove & PopBack ---\n";
    DynamicArray<int> arr;
    arr.append(10);
    arr.append(20);
    arr.append(30);
    arr.append(40);
    arr.append(50); // [10, 20, 30, 40, 50]
    
    // Remove from middle
    arr.remove(2); // Should remove 30 -> [10, 20, 40, 50]
    EXPECT_EQ(4, arr.size(), "Size is 4 after middle remove");
    EXPECT_EQ(10, arr.get(0), "Element 0 is 10");
    EXPECT_EQ(20, arr.get(1), "Element 1 is 20");
    EXPECT_EQ(40, arr.get(2), "Element 2 is 40");
    EXPECT_EQ(50, arr.get(3), "Element 3 is 50");
    
    // Remove from front
    arr.remove(0); // Should remove 10 -> [20, 40, 50]
    EXPECT_EQ(3, arr.size(), "Size is 3 after front remove");
    EXPECT_EQ(20, arr.get(0), "Element 0 is 20");
    
    // Test popBack
    arr.popBack(); // Should remove 50 -> [20, 40]
    EXPECT_EQ(2, arr.size(), "Size is 2 after popBack");
    EXPECT_EQ(40, arr.get(1), "Element 1 is 40");
    
    // Pop until empty
    arr.popBack();
    arr.popBack();
    EXPECT_EQ(0, arr.size(), "Size is 0 after popping all");
    
    bool exceptionThrown = false;
    try {
        arr.popBack();
    } catch (const std::out_of_range& e) {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown, "popBack() throws out_of_range on empty array");
}

int main() {
    std::cout << "Starting DynamicArray Tests...\n";
    
    // Run the tests
    test_initial_state();
    test_custom_capacity();
    test_element_access();
    test_append_and_resize();
    test_insert();
    test_remove_and_pop();
    
    // Print Summary
    std::cout << "\n==============================\n";
    std::cout << "Tests Passed: " << testsPassed << " / " << testCaseCount << "\n";
    
    if (testsFailed > 0) {
        std::cout << "SOME TESTS FAILED!\n";
        return 1;
    }
    std::cout << "ALL TESTS PASSED!\n";
    return 0;
}
