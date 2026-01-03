#include <iostream>
#include <string>
#include <cassert>
#include <sstream>
#include "avl_tree.hpp"

// --- simple test framework ---
void run_test(const std::string& name, void (*test_func)()) {
    std::cout << "Running test: " << name << "... ";
    try {
        test_func();
        std::cout << "[OK]" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[FAIL] Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "[FAIL] Unknown error" << std::endl;
    }
}

void assert_true(bool condition, const std::string& msg) {
    if (!condition) {
        std::cerr << "Assertion failed: " << msg << std::endl;
        throw std::runtime_error(msg);
    }
}

void assert_equal(int a, int b, const std::string& msg) {
    if (a != b) {
        std::cerr << "Assertion failed: " << a << " != " << b << " (" << msg << ")" << std::endl;
        throw std::runtime_error(msg);
    }
}

// --- Tests ---

void test_insert_and_find() {
    avl_tree<int, std::string> tree;
    tree.insert(10, "ten");
    tree.insert(20, "twenty");
    
    // insert using operator[]
    tree[5] = "five";
    tree[15] = "fifteen";
    tree[25] = "twenty-five";
    tree[30] = "thirty";
    tree[40] = "forty";
    tree[50] = "fifty";
    tree[60] = "sixty";
    tree[70] = "seventy";

    //std::cout << "\nCurrent tree structure:\n";

    //tree.print();

    // check existing keys
    std::string val;
    assert_true(tree.search(10, val), "Should find 10");
    assert_true(val == "ten", "Value for 10 incorrect");
    
    assert_true(tree.search(5, val), "Should find 5");
    assert_true(val == "five", "Value for 5 incorrect");

    // check non-existing key
    assert_true(!tree.search(99, val), "Should not find 99");
}

void test_update_existing() {
    avl_tree<int, int> tree;
    tree.insert(1, 100);
    tree.insert(1, 200);

    assert_equal(tree[1], 200, "Insert duplicate key should update value");
    
    tree[1] = 300; 
    assert_equal(tree[1], 300, "Operator[] should update value");
}

void test_rotations_and_structure() {
    
    avl_tree<int, int> tree;
    // Forcing rotation (Left-Left case -> Right Rotation)
    tree.insert(3, 3);
    tree.insert(2, 2);
    tree.insert(1, 1);

    std::vector<std::pair<int, int>> vec;
    tree.to_vector(vec);

    assert_equal(vec.size(), 3, "Size should be 3");
    assert_equal(vec[0].first, 1, "First element should be 1");
    assert_equal(vec[1].first, 2, "Second element should be 2");
    assert_equal(vec[2].first, 3, "Third element should be 3");
    
    const auto& c_tree = tree;
    assert_equal(c_tree[2], 2, "Const operator[] failed");
}

void test_remove() {
    avl_tree<int, int> tree;
    tree.insert(10, 10); // root
    tree.insert(5, 5);   // left
    tree.insert(15, 15); // right
    tree.insert(2, 2);   // leaf

    // deleting leaf node
    tree.remove(2);
    int dummy;
    assert_true(!tree.search(2, dummy), "2 should be removed");

    // deleting node with one child
    tree.remove(10);
    assert_true(!tree.search(10, dummy), "10 should be removed");
    assert_true(tree.search(5, dummy), "5 should remain");
    assert_true(tree.search(15, dummy), "15 should remain");
    
    // checking tree structure
    std::vector<std::pair<int, int>> vec;
    tree.to_vector(vec);
    assert_equal(vec.size(), 2, "Should have 2 elements left");
    assert_equal(vec[0].first, 5, "Order check 1");
    assert_equal(vec[1].first, 15, "Order check 2");
}

void test_copy_constructor() {
    avl_tree<int, int> tree1;
    tree1.insert(10, 10);
    tree1.insert(20, 20);

    avl_tree<int, int> tree2 = tree1; // Copy constructor

    tree1[10] = 999; 
    tree1.remove(20); 
    // copy should remain unchanged
    assert_equal(tree2[10], 10, "Copy should not be modified by original");
    int val;
    assert_true(tree2.search(20, val), "Copy should still have removed element");
}

void test_maxinfo_selector() {
    avl_tree<std::string, int> tree;
    tree.insert("a", 10);
    tree.insert("b", 30);
    tree.insert("c", 20);
    tree.insert("d", 5);

    // should return 2 values (Info): b(30), c(20)
    auto result = maxinfo_selector(tree, 2);

    assert_equal(result.size(), 2, "Selector size incorrect");
    assert_true(result[0].first == "b", "First max should be b");
    assert_true(result[1].first == "c", "Second max should be c");
}

void test_maxinfo_selector_again() {
    avl_tree<std::string, int> tree;
    tree.insert("a", 10);
    tree.insert("b", 30);
    tree.insert("c", 30);
    tree.insert("d", 30);

    // should return 2 values (Info): b(30), c(20)
    auto result = maxinfo_selector(tree, 2);

    assert_equal(result.size(), 2, "Selector size incorrect");
    assert_true(result[0].first == "b", "First max should be b");
    assert_true(result[1].first == "c", "Second max should be c");
}

void test_complex_rotations() {
    // Test double rotations (LR and RL cases)
    
    // Case LR: 30 -> 10 -> 20
    avl_tree<int, int> tree_lr;
    tree_lr.insert(30, 1);
    tree_lr.insert(10, 1);
    tree_lr.insert(20, 1); 

    int val;
    assert_true(tree_lr.search(20, val), "LR Case: 20 missing");
    assert_true(tree_lr.search(10, val), "LR Case: 10 missing");
    assert_true(tree_lr.search(30, val), "LR Case: 30 missing");

    // Case RL: 10 -> 30 -> 20
    avl_tree<int, int> tree_rl;
    tree_rl.insert(10, 1);
    tree_rl.insert(30, 1);
    tree_rl.insert(20, 1);

    assert_true(tree_rl.search(20, val), "RL Case: 20 missing");
    assert_true(tree_rl.search(10, val), "RL Case: 10 missing");
    assert_true(tree_rl.search(30, val), "RL Case: 30 missing");
}

void test_empty_edge_cases() {
    avl_tree<int, int> tree;

    // 1. Remove from empty tree
    try {
        tree.remove(5);
    } catch (...) {
        assert_true(false, "Remove on empty tree threw exception");
    }

    // 2. Clear empty tree
    tree.clear(); 
    
    // 3. Search empty tree
    int val;
    assert_true(!tree.search(10, val), "Search in empty tree returned true");

    // 4. Operator[] on empty tree (should insert)
    tree[100] = 50;
    assert_equal(tree[100], 50, "Operator[] failed on empty tree");
}

void test_stress_insert_remove() {
    avl_tree<int, int> tree;
    const int limit = 1000;

    // Insert 1000 items
    for (int i = 0; i < limit; ++i) {
        tree.insert(i, i * 2);
    }

    std::vector<std::pair<int, int>> vec;
    tree.to_vector(vec);
    assert_equal((int)vec.size(), limit, "Stress test size mismatch");

    // Verify BST order
    bool sorted = true;
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        if (vec[i].first >= vec[i+1].first) {
            sorted = false;
            break;
        }
    }
    assert_true(sorted, "Tree is not sorted after stress insert");

    // Remove half (even keys)
    for (int i = 0; i < limit; i += 2) { 
        tree.remove(i);
    }

    vec.clear();
    tree.to_vector(vec);
    assert_equal((int)vec.size(), limit / 2, "Size mismatch after mass remove");
    assert_equal(vec[0].first, 1, "First element check failed");
}

void test_clear_logic() {
    avl_tree<int, std::string> tree;
    tree.insert(1, "one");
    tree.insert(2, "two");

    tree.clear();

    std::vector<std::pair<int, std::string>> vec;
    tree.to_vector(vec);
    assert_equal((int)vec.size(), 0, "Tree not empty after clear");
    
    // Reuse after clear
    tree.insert(10, "ten");
    assert_equal(tree[10] == "ten", true, "Reuse after clear failed");
}

void test_double_types() {
    // Ensure template works for non-int types
    avl_tree<double, double> tree;
    tree.insert(1.1, 10.0);
    tree.insert(1.2, 20.0);
    tree.insert(1.0, 5.0); 

    std::vector<std::pair<double, double>> vec;
    tree.to_vector(vec);

    assert_true(vec[0].first < vec[1].first, "Double keys sorting error 1");
    assert_true(vec[1].first < vec[2].first, "Double keys sorting error 2");
}

void test_other_functions() {
    avl_tree<int, int> tree;
    assert_true(tree.empty(), "New tree should be empty");
    assert_equal(tree.size(), 0, "New tree size should be 0");

    tree.insert(1, 100);
    tree.insert(2, 200);

    assert_true(!tree.empty(), "Tree should not be empty after inserts");
    assert_equal(tree.size(), 2, "Tree size should be 2 after inserts");
}

void test_count_words_empty() {
    std::istringstream iss("");
    avl_tree<std::string, int> result = count_words(iss);
    
    assert_true(result.empty(), "Tree should be empty for empty input");
}

void test_count_words_single_word() {
    std::istringstream iss("hello");
    avl_tree<std::string, int> result = count_words(iss);
    
    assert_true(result.size() == 1, "Should have exactly one word");
    
    int count = 0;
    bool found = result.search("hello", count);
    assert_true(found, "Should find 'hello'");
    assert_equal(count, 1, "'hello' should appear once");
}

void test_count_words_multiple_same_word() {
    std::istringstream iss("hello hello hello");
    avl_tree<std::string, int> result = count_words(iss);
    
    assert_true(result.size() == 1, "Should have exactly one unique word");
    
    int count = 0;
    result.search("hello", count);
    assert_equal(count, 3, "'hello' should appear 3 times");
}

void test_count_words_different_words() {
    std::istringstream iss("apple banana apple cherry banana apple");
    avl_tree<std::string, int> result = count_words(iss);
    
    assert_true(result.size() == 3, "Should have 3 unique words");
    
    int count;
    
    result.search("apple", count);
    assert_equal(count, 3, "'apple' should appear 3 times");
    
    result.search("banana", count);
    assert_equal(count, 2, "'banana' should appear 2 times");
    
    result.search("cherry", count);
    assert_equal(count, 1, "'cherry' should appear 1 time");
}

void test_count_words_case_insensitive() {
    std::istringstream iss("Hello HELLO hello");
    avl_tree<std::string, int> result = count_words(iss);
    
    assert_true(result.size() == 1, "Should have exactly one unique word (case insensitive)");
    
    int count = 0;
    result.search("hello", count);
    assert_equal(count, 3, "'hello' should appear 3 times (case insensitive)");
}

void test_count_words_with_punctuation() {
    std::istringstream iss("Hello, world! World... hello!");
    avl_tree<std::string, int> result = count_words(iss);
    
    assert_true(result.size() == 2, "Should have 2 unique words");
    
    int count;
    
    result.search("hello", count);
    assert_equal(count, 2, "'hello' should appear 2 times (without punctuation)");
    
    result.search("world", count);
    assert_equal(count, 2, "'world' should appear 2 times (without punctuation)");
}

void test_count_words_numbers() {
    std::istringstream iss("test123 456test test123 789");
    avl_tree<std::string, int> result = count_words(iss);
    
    assert_true(result.size() == 3, "Should have 3 unique words with numbers");
    
    int count;
    
    result.search("test123", count);
    assert_equal(count, 2, "'test123' should appear 2 times");
    
    result.search("456test", count);
    assert_equal(count, 1, "'456test' should appear 1 time");
    
    result.search("789", count);
    assert_equal(count, 1, "'789' should appear 1 time");
}

void test_count_words_mixed_whitespace() {
    std::istringstream iss("  one  two\nthree\tone\n\nfour  ");
    avl_tree<std::string, int> result = count_words(iss);
    
    assert_true(result.size() == 4, "Should have 4 unique words");
    
    int count;
    
    result.search("one", count);
    assert_equal(count, 2, "'one' should appear 2 times");
    
    result.search("two", count);
    assert_equal(count, 1, "'two' should appear 1 time");
    
    result.search("three", count);
    assert_equal(count, 1, "'three' should appear 1 time");
    
    result.search("four", count);
    assert_equal(count, 1, "'four' should appear 1 time");
}

void test_count_words_special_chars_only() {
    std::istringstream iss("!!! ??? ... ,,, ---");
    avl_tree<std::string, int> result = count_words(iss);
    
    assert_true(result.empty(), "Tree should be empty for only special characters");
}