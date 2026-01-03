#include "avl_tree.hpp"
#include "avl_tree_test.hpp"

int main() {
    std::cout << "--- STARTING UNIT TESTS ---" << std::endl;
    
    run_test("Insert and Find", test_insert_and_find);
    run_test("Update Existing", test_update_existing);
    run_test("Rotations and Structure", test_rotations_and_structure);
    run_test("Remove Logic", test_remove);
    run_test("Deep Copy", test_copy_constructor);
    run_test("Helper: Max Info Selector", test_maxinfo_selector);
    run_test("Helper: Max Info Selector Again", test_maxinfo_selector_again);
    run_test("Stress Test: Insert and Remove", test_stress_insert_remove);
    run_test("Clear Logic", test_clear_logic);
    run_test("Double Types", test_double_types);
    run_test("Complex Rotations", test_complex_rotations);
    run_test("Empty Edge Cases", test_empty_edge_cases);
    run_test("Other Functions", test_other_functions);
    run_test("Count Words - Empty Input", test_count_words_empty);
    run_test("Count Words - Single Word", test_count_words_single_word);
    run_test("Count Words - Multiple Words", test_count_words_multiple_same_word);
    run_test("Count Words - Different Words", test_count_words_different_words);
    run_test("Count Words - Case Insensitive", test_count_words_case_insensitive);
    run_test("Count Words - With Punctuation", test_count_words_with_punctuation);
    run_test("Count Words - Numbers", test_count_words_numbers);


    std::cout << "--- ALL TESTS FINISHED ---" << std::endl;
    return 0;
}