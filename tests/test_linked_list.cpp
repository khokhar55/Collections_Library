#include <iostream>
#include <string>
#include <stdexcept>
#include "../include/LinkedList.h"

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
    LinkedList<int> list;
    EXPECT_EQ(0, list.getSize(), "Initial size is 0");
    EXPECT_TRUE(list.isEmpty(), "List is empty initially");
}

void test_prepend_and_append() {
    std::cout << "\n--- Testing Prepend & Append ---\n";
    LinkedList<int> list;
    
    list.prepend(10);
    EXPECT_EQ(1, list.getSize(), "Size is 1 after prepend");
    
    // The second append will crash because tail is null!
    LinkedList<int> list2;
    list2.append(20);
    EXPECT_EQ(1, list2.getSize(), "Size is 1 after first append");
    
    // Crash here:
    list2.append(30);
    EXPECT_EQ(2, list2.getSize(), "Size is 2 after second append");
}

void test_insert_and_remove() {
    std::cout << "\n--- Testing Insert & Remove ---\n";
    LinkedList<int> list;
    
    list.append(10);
    list.append(20);
    list.append(30);
    list.append(40);
    list.append(50);
    
    list.insert(2, 25);
    EXPECT_EQ(6, list.getSize(), "Size is 6 after insert");
    EXPECT_EQ(25, list.get(2), "Element 2 is 25");
    
    // Testing the buggy remove method!
    // We are trying to remove element at index 2 (value 25)
    list.remove(2);
    
    // Since we accidentally broke the link and deleted the node without bypassing it,
    // the list now has a dangling pointer.
    // The next node after 20 (index 1) will be reading from the freed memory of 25!
    EXPECT_EQ(5, list.getSize(), "Size is 5 after remove");
    EXPECT_EQ(30, list.get(2), "Element 2 should be 30 after removing 25");
}

void test_search() {
    std::cout << "\n--- Testing Search ---\n";
    LinkedList<int> list;
    list.append(100);
    list.append(200);
    list.append(300);

    // This will pass because 100 is at the head
    EXPECT_EQ(0, list.indexOf(100), "Index of 100 is 0");
    EXPECT_TRUE(list.contains(100), "List contains 100");

    // These will fail because of the premature return bug!
    EXPECT_EQ(1, list.indexOf(200), "Index of 200 is 1");
    EXPECT_TRUE(list.contains(200), "List contains 200");
    EXPECT_EQ(2, list.indexOf(300), "Index of 300 is 2");
    EXPECT_EQ(-1, list.indexOf(999), "Index of 999 is -1");
}

int main() {
    std::cout << "Starting LinkedList Tests...\n";
    
    // Run the tests
    test_initial_state();
    test_prepend_and_append();
    test_insert_and_remove();
    test_search();
    
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
