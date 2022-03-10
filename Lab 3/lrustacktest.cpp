/**
 * @author ECE 3058 TAs
 */

#include <stdio.h>
#include "LruStack.h"

int test_num = 1;

/**
 * Function to assert equality for test cases
 * 
 * @param test_num number of this test
 * @param expected the expected value
 * @param actual the actual value
 */
void assert_equal(int test_num, int expected, int actual) {
    if (expected == actual) {
        printf("Test%d Succeeded!\n", test_num);
    } else {
        printf("Test%d Failed: expected %d, but got %d\n", test_num, expected, actual);
    }
}

/**
 * Function to run tests (written by TAs) to test your LRU stack implementation
 */
void run_ta_tests() {
    // Initialize LRU stack
    int size = 8;
    LruStack stack;

    // For testing, LRU stack is initialized with 0 as MRU and 1 as LRU.
    for (int i = (size - 1); i >= 0; i--) {
        stack.setMru(i);
    }
    
    // Test whether (size - 1) is the LRU
    assert_equal(test_num++, size - 1, stack.getLru());

    // Sequentially mark each LRU as MRU and make sure LRU rotates. 
    for (int i = (size - 1); i >= 1; i--) {
        stack.setMru(i);
        assert_equal(test_num++, i - 1, stack.getLru());
    }

    // Make sure marking anything MRU works
    stack.setMru(4);
    for (int i = 0; i < (size - 1); i++) {
        stack.setMru(stack.getLru());
    }
    assert_equal(test_num++, 4, stack.getLru());
}

/**
 * Function to run tests (written by YOU) to test your LRU stack implementation
 */
void run_student_tests() {
    ////////////////////////////////////////////////////////////////////
    //  Optional: Add any additional tests for your LRU stack implementation. 
    ////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////
    //  End of your code   
    ////////////////////////////////////////////////////////////////////
}

int main() {
    printf("------- Running TA tests -------\n");
    run_ta_tests();
    printf("------- Running Student tests -------\n");
    run_student_tests();
    return 0;
}