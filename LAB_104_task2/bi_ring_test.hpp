#include "bi_ring.hpp" 
#include <string>
#include <vector>

//functions to manage unit_tests for bi_ring, join and shuffle
template <typename Key, typename Info> 
std::vector<std::pair<Key,Info>> toVector(const bi_ring<Key,Info>& r) { // function in order to change bi_ring to vector for easy testing
    std::vector<std::pair<Key,Info>> out;
    if (r.is_empty()) return out;

    auto it = r.begin();
    do {
        out.push_back({it.key(), it.info()});
        ++it;
    } while (it != r.begin());

    return out;
}

template <typename Key, typename Info>
void assertEqual(const std::vector<std::pair<Key,Info>>& a, const std::vector<std::pair<Key,Info>>& b, const std::string& testName) // simple assert function for tests
{
    if (a == b)
        std::cout << "[OK] " << testName << "\n";
    else {
        std::cout << "[FAIL] " << testName << "\n";
        std::cout << "Expected:\n";
        for (auto& x : b) std::cout << "(" << x.first << "," << x.second << ") ";
        std::cout << "\nGot:\n";
        for (auto& x : a) std::cout << "(" << x.first << "," << x.second << ") ";
        std::cout << "\n";
    }
}

//bi_ring tests
void testBiRingBasic() {
    bi_ring<int, std::string> r;

    r.push_back(1, "one");
    r.push_back(2, "two");
    r.push_front(0, "zero");

    auto out = toVector(r);
    std::vector<std::pair<int, std::string>> expected = {
        {0, "zero"},
        {1, "one"},
        {2, "two"}
    };

    assertEqual(out, expected, "BiRingBasic");
}

void testBiRingCopy() {
    bi_ring<int, std::string> r1;
    r1.push_back(1, "one");
    r1.push_back(2, "two");

    bi_ring<int, std::string> r2 = r1; // copy constructor

    auto out = toVector(r2);
    std::vector<std::pair<int, std::string>> expected = {
        {1, "one"},
        {2, "two"}
    };

    assertEqual(out, expected, "BiRingCopy");
}

void testBiRingAssignment() {
    bi_ring<int, std::string> r1;
    r1.push_back(1, "one");
    r1.push_back(2, "two");

    bi_ring<int, std::string> r2;
    r2 = r1; // assignment operator

    auto out = toVector(r2);
    std::vector<std::pair<int, std::string>> expected = {
        {1, "one"},
        {2, "two"}
    };

    assertEqual(out, expected, "BiRingAssignment");
}

void testIterators() {
    bi_ring<int, std::string> r;
    r.push_back(1, "one");
    r.push_back(2, "two");
    r.push_back(3, "three");

    std::vector<std::pair<int, std::string>> forward;
    auto it_forward = r.begin();
    do {
        forward.push_back({it_forward.key(), it_forward.info()});
        ++it_forward;
    } while (it_forward != r.begin());

    std::vector<std::pair<int, std::string>> backward;
    auto it_backward = r.begin();
    do {
        --it_backward;
        backward.push_back({it_backward.key(), it_backward.info()});
    } while (it_backward != r.begin());

    std::vector<std::pair<int, std::string>> expected_forward = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };

    std::vector<std::pair<int, std::string>> expected_backward = {
        {3, "three"},
        {2, "two"},
        {1, "one"}
    };

    assertEqual(forward, expected_forward, "IteratorForward");
    assertEqual(backward, expected_backward, "IteratorBackward");
}

void testInsert(){
    bi_ring<int, int> r;
    r.push_back(1,10);
    r.push_back(2,20);
    r.push_back(3,30);

    bi_ring<int, int>::iterator it = r.begin();
    ++it; // pointing to element with key 2
    r.insert(it, 4,40); // insert before begin()
    it= r.begin();
    --it;
    r.insert(it, 5,50); // insert before last element (which is 3)

    auto out = toVector(r);
    std::vector<std::pair<int,int>> expected = {
        {1,10},
        {4,40},
        {2,20},
        {5,50},
        {3,30}
    };

    assertEqual(out, expected, "InsertTest");
}

void eraseTest(){
    bi_ring<int, int> r;
    r.push_back(1,10);
    r.push_back(2,20);
    r.push_back(3,30);

    bi_ring<int, int>::iterator it = r.begin();
    ++it; // pointing to element with key 2
    r.erase(it); // erase element with key 2

    auto out = toVector(r);
    std::vector<std::pair<int,int>> expected = {
        {1,10},
        {3,30}
    };

    assertEqual(out, expected, "EraseTest");
}

//join tests
void testJoinBothEmpty() {
    bi_ring<int,int> a, b;

    auto res = join(a, b);
    auto out = toVector(res);

    assertEqual(out, {}, "JoinBothEmpty");
}

void testJoinFirstEmpty() {
    bi_ring<int,int> a, b;
    b.push_back(1, 10);
    b.push_back(2, 20);

    auto res = join(a, b);
    auto out = toVector(res);

    std::vector<std::pair<int,int>> expected = {
        {1,10},
        {2,20}
    };

    assertEqual(out, expected, "JoinFirstEmpty");
}
void testJoinSecondEmpty() {
    bi_ring<int,int> a, b;
    a.push_back(1, 5);
    a.push_back(2, 7);

    auto res = join(a, b);
    auto out = toVector(res);

    std::vector<std::pair<int,int>> expected = {
        {1,5},
        {2,7}
    };

    assertEqual(out, expected, "JoinSecondEmpty");
}
void testJoinNoCommonKeys() {
    bi_ring<int,int> a, b;

    a.push_back(1,10);
    a.push_back(2,20);

    b.push_back(3,30);
    b.push_back(4,40);

    auto res = join(a, b);
    auto out = toVector(res);

    std::vector<std::pair<int,int>> expected = {
        {1,10},
        {2,20},
        {3,30},
        {4,40}
    };

    assertEqual(out, expected, "JoinNoCommonKeys");
}
void testJoinExample() {
    bi_ring<std::string,int> first, second;

    first.push_back("uno",1);
    first.push_back("due",1);
    first.push_back("tre",2);
    first.push_back("quattro",1);

    second.push_back("due",1);
    second.push_back("tre",1);
    second.push_back("quattro",3);
    second.push_back("cinque",5);

    auto res = join(first, second);
    auto out = toVector(res);

    std::vector<std::pair<std::string,int>> expected = {
        {"uno",1},
        {"due",2},
        {"tre",3},
        {"quattro",4},
        {"cinque",5}
    };

    assertEqual(out, expected, "JoinExample");
}
void testJoinSomeCommon() {
    bi_ring<int,int> a, b;

    a.push_back(1,10);
    a.push_back(2,20);
    a.push_back(3,30);

    b.push_back(2,5);  // wspólny
    b.push_back(4,40); // nowy

    auto res = join(a, b);
    auto out = toVector(res);

    std::vector<std::pair<int,int>> expected = {
        {1,10},
        {2,25}, // 20 + 5
        {3,30},
        {4,40}
    };

    assertEqual(out, expected, "JoinSomeCommon");
}


//shuffle tests
void testShuffleBothEmpty() {
    bi_ring<int,char> a, b;

    auto res = shuffle(a, 2, b, 3, 4);
    auto out = toVector(res);

    assertEqual(out, {}, "BothEmpty");
}

void testShuffleFirstEmpty() {
    bi_ring<int,char> a, b;
    b.push_back(1,'A');
    b.push_back(2,'B');

    auto res = shuffle(a, 5, b, 1, 3);
    auto out = toVector(res);

    std::vector<std::pair<int,char>> expected = {
        {1,'A'}, {2,'B'}, {1,'A'}
    };

    assertEqual(out, expected, "FirstEmpty");
}

void testShuffleSecondEmpty() {
    bi_ring<int,char> a, b;
    a.push_back(10,'X');
    a.push_back(20,'Y');

    auto res = shuffle(a, 2, b, 5, 2);
    auto out = toVector(res);

    std::vector<std::pair<int,char>> expected = {
        {10,'X'}, {20,'Y'},
        {10,'X'}, {20,'Y'}
    };

    assertEqual(out, expected, "SecondEmpty");
}

void testShuffleBasic() {
    bi_ring<int,char> a, b;
    a.push_back(1,'A');
    a.push_back(2,'B');
    a.push_back(3,'C');

    b.push_back(10,'X');
    b.push_back(20,'Y');

    auto res = shuffle(a, 2, b, 1, 2);
    auto out = toVector(res);

    std::vector<std::pair<int,char>> expected = {
        {1,'A'}, {2,'B'}, {10,'X'},
        {3,'C'}, {1,'A'}, {20,'Y'}
    };

    assertEqual(out, expected, "BasicShuffle");
}

void testShuffleWrapAround() {
    bi_ring<int,char> a, b;
    a.push_back(5,'Q');
    b.push_back(7,'Z');
    b.push_back(8,'W');

    auto res = shuffle(a, 3, b, 2, 2);
    auto out = toVector(res);

    std::vector<std::pair<int,char>> expected = {
        {5,'Q'}, {5,'Q'}, {5,'Q'},
        {7,'Z'}, {8,'W'},
        {5,'Q'}, {5,'Q'}, {5,'Q'},
        {7,'Z'}, {8,'W'}
    };

    assertEqual(out, expected, "WrapAround");
}

void testShuffleZeroCounts() {
    bi_ring<int,char> a, b;
    a.push_back(1,'A');
    b.push_back(2,'B');

    auto res = shuffle(a, 0, b, 0, 5);
    auto out = toVector(res);

    assertEqual(out, {}, "ZeroCounts");
}