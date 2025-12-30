#include <iostream>
#include <cassert>
#include <stdexcept>
#include "sequence.hpp"
#include "split.hpp"

// Test 1: Default constructor and is_empty
void test_default_constructor_and_is_empty() {
    std::cout << "Test 1: Default constructor and is_empty\n";
    Sequence<int, std::string> seq;
    assert(seq.is_empty() == true);
    assert(seq.size() == 0);
    std::cout << "PASSED\n\n";
}

// Test 2: push_front and pop_front
void test_push_front_and_pop_front() {
    std::cout << "Test 2: push_front and pop_front\n";
    Sequence<int, std::string> seq;
    
    seq.push_front(1, "one");
    assert(seq.is_empty() == false);
    assert(seq.size() == 1);
    
    seq.push_front(2, "two");
    assert(seq.size() == 2);
    
    assert(seq.pop_front() == true);
    assert(seq.size() == 1);
    
    assert(seq.pop_front() == true);
    assert(seq.is_empty() == true);
    
    assert(seq.pop_front() == false); // empty list
    std::cout << "PASSED\n\n";
}

// Test 3: push_back and pop_back
void test_push_back_and_pop_back() {
    std::cout << "Test 3: push_back and pop_back\n";
    Sequence<int, std::string> seq;
    
    seq.push_back(1, "one");
    assert(seq.is_empty() == false);
    assert(seq.size() == 1);
    
    seq.push_back(2, "two");
    assert(seq.size() == 2);
    
    assert(seq.pop_back() == true);
    assert(seq.size() == 1);
    
    assert(seq.pop_back() == true);
    assert(seq.is_empty() == true);
    
    assert(seq.pop_back() == false); // empty list
    std::cout << "PASSED\n\n";
}

// Test 4: Copy constructor
void test_copy_constructor() {
    std::cout << "Test 4: Copy constructor\n";
    Sequence<int, std::string> original;
    original.push_back(1, "one");
    original.push_back(2, "two");
    original.push_back(3, "three");
    
    Sequence<int, std::string> copy(original);
    
    assert(copy.size() == 3);
    assert(copy.get_key_at(0) == 1);
    assert(copy.get_info_at(0) == "one");
    assert(copy.get_key_at(1) == 2);
    assert(copy.get_info_at(1) == "two");
    assert(copy.get_key_at(2) == 3);
    assert(copy.get_info_at(2) == "three");
    
    // Verify original is unchanged
    assert(original.size() == 3);
    std::cout << "PASSED\n\n";
}

// Test 5: Assignment operator
void test_assignment_operator() {
    std::cout << "Test 5: Assignment operator\n";
    Sequence<int, std::string> original;
    original.push_back(1, "one");
    original.push_back(2, "two");
    
    Sequence<int, std::string> assigned;
    assigned = original;
    
    assert(assigned.size() == 2);
    assert(assigned.get_key_at(0) == 1);
    assert(assigned.get_info_at(0) == "one");
    assert(assigned.get_key_at(1) == 2);
    assert(assigned.get_info_at(1) == "two");
    
    // Self-assignment test
    assigned = assigned;
    assert(assigned.size() == 2);
    std::cout << "PASSED\n\n";
}

// Test 6: insert_at and remove_at
void test_insert_at_and_remove_at() {
    std::cout << "Test 6: insert_at and remove_at\n";
    Sequence<int, std::string> seq;
    
    // Insert at beginning
    assert(seq.insert_at(1, "one", 0) == true);
    assert(seq.size() == 1);
    
    // Insert at end
    assert(seq.insert_at(3, "three", 1) == true);
    assert(seq.size() == 2);
    
    // Insert in middle
    assert(seq.insert_at(2, "two", 1) == true);
    assert(seq.size() == 3);
    
    // Verify order
    assert(seq.get_key_at(0) == 1);
    assert(seq.get_key_at(1) == 2);
    assert(seq.get_key_at(2) == 3);
    
    // Remove from middle
    assert(seq.remove_at(1) == true);
    assert(seq.size() == 2);
    assert(seq.get_key_at(0) == 1);
    assert(seq.get_key_at(1) == 3);
    
    // Remove invalid positions
    assert(seq.remove_at(-1) == false);
    assert(seq.remove_at(5) == false);
    
    // Insert invalid positions
    assert(seq.insert_at(4, "four", -1) == false);
    assert(seq.insert_at(4, "four", 5) == false);
    std::cout << "PASSED\n\n";
}

// Test 7: get_key_at and get_info_at with exceptions
void test_get_at_methods() {
    std::cout << "Test 7: get_key_at and get_info_at with exceptions\n";
    Sequence<int, std::string> seq;
    seq.push_back(1, "one");
    seq.push_back(2, "two");
    
    assert(seq.get_key_at(0) == 1);
    assert(seq.get_info_at(0) == "one");
    assert(seq.get_key_at(1) == 2);
    assert(seq.get_info_at(1) == "two");
    
    // Test exceptions
    try {
        seq.get_key_at(-1);
        assert(false); // Should not reach here
    } catch (const std::out_of_range& e) {
        assert(true);
    }
    
    try {
        seq.get_info_at(5);
        assert(false); // Should not reach here
    } catch (const std::out_of_range& e) {
        assert(true);
    }
    std::cout << "PASSED\n\n";
}

// Test 8: clear method
void test_clear() {
    std::cout << "Test 8: clear method\n";
    Sequence<int, std::string> seq;
    seq.push_back(1, "one");
    seq.push_back(2, "two");
    seq.push_back(3, "three");
    
    assert(seq.size() == 3);
    seq.clear();
    assert(seq.is_empty() == true);
    assert(seq.size() == 0);
    
    // Clear empty list
    seq.clear();
    assert(seq.is_empty() == true);
    std::cout << "PASSED\n\n";
}

// Test 9: reverse method
void test_reverse() {
    std::cout << "Test 9: reverse method\n";
    Sequence<int, std::string> seq;
    seq.push_back(1, "one");
    seq.push_back(2, "two");
    seq.push_back(3, "three");
    
    seq.reverse();
    
    assert(seq.get_key_at(0) == 3);
    assert(seq.get_info_at(0) == "three");
    assert(seq.get_key_at(1) == 2);
    assert(seq.get_info_at(1) == "two");
    assert(seq.get_key_at(2) == 1);
    assert(seq.get_info_at(2) == "one");
    
    // Reverse empty list
    Sequence<int, std::string> empty_seq;
    empty_seq.reverse();
    assert(empty_seq.is_empty() == true);
    std::cout << "PASSED\n\n";
}

// Test 10: update_info method
void test_update_info() {
    std::cout << "Test 10: update_info method\n";
    Sequence<int, std::string> seq;
    seq.push_back(1, "one");
    seq.push_back(2, "two");
    seq.push_back(1, "one_again");
    
    // Update first occurrence
    seq.update_info(1, "ONE", 1);
    assert(seq.get_info_at(0) == "ONE");
    assert(seq.get_info_at(2) == "one_again");
    
    // Update second occurrence
    seq.update_info(1, "ONE_AGAIN", 2);
    assert(seq.get_info_at(2) == "ONE_AGAIN");
    
    // Test exception for invalid occurrence
    try {
        seq.update_info(1, "test", 0);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(true);
    }
    std::cout << "PASSED\n\n";
}

// Test 11: split_pos basic functionality
void test_split_pos_basic() {
    std::cout << "Test 11: split_pos basic functionality\n";
    Sequence<int, std::string> seq;
    for (int i = 1; i <= 6; i++) {
        seq.push_back(i, "value_" + std::to_string(i));
    }
    
    Sequence<int, std::string> seq1;
    Sequence<int, std::string> seq2;
    
    split_pos(seq, 0, 2, 1, 2, seq1, seq2);
    
    assert(seq1.size() == 4); // 2 elements * 2 iterations
    assert(seq2.size() == 2); // 1 element * 2 iterations
    assert(seq.size() == 0);  // All elements removed
    
    // Verify seq1 contents
    assert(seq1.get_key_at(0) == 1);
    assert(seq1.get_key_at(1) == 2);
    assert(seq1.get_key_at(2) == 4);
    assert(seq1.get_key_at(3) == 5);
    
    // Verify seq2 contents
    assert(seq2.get_key_at(0) == 3);
    assert(seq2.get_key_at(1) == 6);
    std::cout << "PASSED\n\n";
}

// Test 12: split_pos with different start position
void test_split_pos_start_position() {
    std::cout << "Test 12: split_pos with different start position\n";
    Sequence<int, std::string> seq;
    for (int i = 1; i <= 6; i++) {
        seq.push_back(i, "value_" + std::to_string(i));
    }
    
    Sequence<int, std::string> seq1;
    Sequence<int, std::string> seq2;

    //seq.print();

    split_pos(seq, 2, 1, 2, 2, seq1, seq2);

    //seq.print();
    //seq1.print();
    //seq2.print();
    
    assert(seq1.size() == 2);
    assert(seq2.size() == 2);
    assert(seq.size() == 2);
    
    // Verify contents
    assert(seq1.get_key_at(0) == 3);
    assert(seq1.get_key_at(1) == 6);
    assert(seq2.get_key_at(0) == 4);
    assert(seq2.get_key_at(1) == 5);
    std::cout << "PASSED\n\n";
}

// Test 13: split_key basic functionality
void test_split_key_basic() {
    std::cout << "Test 13: split_key basic functionality\n";
    Sequence<int, std::string> seq;
    seq.push_back(1, "one");
    seq.push_back(5, "five");
    seq.push_back(2, "two");
    seq.push_back(5, "five_again");
    seq.push_back(3, "three");
    seq.push_back(5, "five_third");
    seq.push_back(6, "six");
    seq.push_back(7, "seven");
    seq.push_back(5, "five_fourth");
    
    Sequence<int, std::string> seq1;
    Sequence<int, std::string> seq2;

    seq.print();
    
    split_key(seq, 5, 2, 1, 1, 2, seq1, seq2);

    seq.print();
    seq1.print();
    seq2.print();
    
    assert(seq1.size() == 2);
    //std::cout<<"HERE: "<<seq2.size()<<std::endl;
    assert(seq2.size() == 2);
    assert(seq.size() == 5); // Remaining elements
    
    // Verify contents
    //std::cout<<"1"<<std::endl;
    assert(seq1.get_key_at(0) == 5);
    assert(seq1.get_info_at(0) == "five_again");
    //std::cout<<"2"<<std::endl;
    assert(seq1.get_key_at(1) == 5);
    assert(seq1.get_info_at(1) == "five_third");
    
    assert(seq2.get_key_at(0) == 3);
    assert(seq.get_key_at(0) == 1); // Remaining element
    assert(seq.get_key_at(3) == 7);
    std::cout << "PASSED\n\n";
}

// Test 14: split_pos exception handling
void test_split_pos_exceptions() {
    std::cout << "Test 14: split_pos exception handling\n";
    Sequence<int, std::string> seq;
    Sequence<int, std::string> seq1;
    Sequence<int, std::string> seq2;
    
    // Test invalid arguments
    try {
        split_pos(seq, -1, 1, 1, 1, seq1, seq2);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(true);
    }
    
    try {
        split_pos(seq, 0, -1, 1, 1, seq1, seq2);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(true);
    }
    
    try {
        split_pos(seq, 0, 1, -1, 1, seq1, seq2);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(true);
    }
    
    try {
        split_pos(seq, 0, 1, 1, -1, seq1, seq2);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(true);
    }
    std::cout << "PASSED\n\n";
}

// Test 15: split_key exception handling
void test_split_key_exceptions() {
    std::cout << "Test 15: split_key exception handling\n";
    Sequence<int, std::string> seq;
    seq.push_back(1, "one");
    
    Sequence<int, std::string> seq1;
    Sequence<int, std::string> seq2;
    
    // Test invalid arguments
    try {
        split_key(seq, 1, -1, 1, 1, 1, seq1, seq2);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(true);
    }
    
    try {
        split_key(seq, 1, 1, -1, 1, 1, seq1, seq2);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(true);
    }
    
    // Test key not found
    try {
        split_key(seq, 99, 1, 1, 1, 1, seq1, seq2);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(true);
    }
    std::cout << "PASSED\n\n";
}

int main() {
    std::cout << "Running unit tests for Sequence and Split...\n\n";
    // - - - - -
    test_default_constructor_and_is_empty();
    test_push_front_and_pop_front();
    test_push_back_and_pop_back();
    test_copy_constructor();
    test_assignment_operator();
    // - - - - -
    test_insert_at_and_remove_at();
    test_get_at_methods();
    test_clear();
    test_reverse();
    test_update_info();
    // - - - - -
    test_split_pos_basic();
    test_split_pos_start_position();
    test_split_key_basic();
    test_split_pos_exceptions();
    test_split_key_exceptions();
    
    std::cout << "All 15 tests passed successfully!\n";
    return 0;
}