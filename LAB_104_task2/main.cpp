#include <iostream>
#include "bi_ring.hpp"
#include "bi_ring_test.hpp"

using namespace std;

int main() {

    cout << "Running bi_ring class tests..." << endl;
    testBiRingBasic();
    testBiRingCopy();
    testBiRingAssignment();
    testIterators();
    testInsert();
    eraseTest();

    cout << "Running bi_ring join tests..." << endl;
    testJoinBothEmpty();
    testJoinFirstEmpty();
    testJoinSecondEmpty();
    testJoinNoCommonKeys();
    testJoinExample();
    testJoinSomeCommon();

    cout << "Running bi_ring shuffle tests..." << endl;
    testShuffleBothEmpty();
    testShuffleFirstEmpty();
    testShuffleSecondEmpty();
    testShuffleBasic();
    testShuffleWrapAround();
    testShuffleZeroCounts();
    cout << "All tests passed!" << endl;
    return 0;
}