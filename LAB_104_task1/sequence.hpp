#pragma once
#include <iostream>
/**
 * @file sequence.hpp
 * @brief Singly linked list container template mapping keys to associated info values.
 *
 * Template parameters:
 *   - Key:   Type used for node keys. Comparison operators (==, >, <) used by some operations
 *            must be defined for this type.
 *   - Info:  Type used for stored information associated with each key.
 *
 * Notes:
 *   - This is a simple singly linked list implementation providing common list operations
 *     and positional access. Many operations are linear-time due to traversal of the list.
 *   - The key and info types use default comparators and logic operations suchh as equality and comparison.
 */

/**
 * @class Sequence
 * @brief A singly linked list of (Key, Info) pairs.
 *
 * Provides basic list functionality: push/pop at front/back, insert/remove at position,
 * access by index, search/update by key occurrence, reversal, copying and printing.
 *
 * Complexity summary (n = number of nodes):
 *   - push_front: O(1)
 *   - pop_front:  O(1)
 *   - push_back:  O(n)
 *   - pop_back:   O(n)
 *   - insert_at:  O(n)
 *   - remove_at:  O(n)
 *   - size:       O(n)
 *   - get_key_at / get_info_at: O(n)
 *   - reverse:    O(n)
 *   - update_info (search by key): O(n)
 *   - copy constructor / assignment: O(n)
 */

/**
 * @struct Node
 * @brief Internal node representing one element in the linked list.
 *
 * Members:
 *   - key:  The node key of type Key.
 *   - info: Associated information of type Info.
 *   - next: Pointer to the next node in the list (nullptr for tail).
 */

template <typename Key, typename Info>
class Sequence {
private:
    struct Node {
        Key key;
        Info info;
        Node* next;
        
        Node(Key k, Info i) : key(k), info(i), next(nullptr) {}
    };
    Node* head;

public:
    Sequence();
    Sequence(const Sequence& other);
    ~Sequence();
    void push_front(const Key& k, const Info& i);
    bool pop_front();
    void push_back(const Key& k, const Info& i);
    bool pop_back();
    bool is_empty() const;
    void clear();
    void print() const;
    Sequence& operator=(const Sequence& other);
    bool insert_at(const Key& k, const Info& i, int position); // returns true if successful
    bool remove_at(int position); // returns true if successful
    unsigned int size() const;
    Key get_key_at(int position) const;
    Info get_info_at(int position) const;
    void reverse();
    void update_info(const Key& k, const Info& new_info, int occurrence=1);
    void subsequence(int start_pos, int length, Sequence<Key, Info>& subseq) const;
    void replace_at(int position, const Key& new_key, const Info& new_info); 
    int find_key_occurrence(const Key& k, int occurrence) const;
};

/**
 * @brief Default constructor. Creates an empty Sequence.
 *
 * Complexity: O(1)
 */
template <typename Key, typename Info>
Sequence<Key, Info>::Sequence() : head(nullptr) {}

/**
 * @brief Copy constructor. Performs a deep copy of `other`.
 *
 * Copies all (Key, Info) pairs from `other` in the same order.
 *
 * Complexity: O(n) where n is other.size().
 */
template <typename Key, typename Info>
Sequence<Key, Info>::Sequence(const Sequence& other) : head(nullptr) {
    Node* current = other.head;
    while (current) {
        push_back(current->key, current->info);
        current = current->next;
    }
}

/**
 * @brief Destructor. Releases all allocated nodes.
 *
 * Traverses the list and deletes each node. After destruction the list is empty.
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
Sequence<Key, Info>::~Sequence() {
    Node* current = head;
    while(current) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
}

/**
 * @brief Insert a new element at the front of the list.
 * @param k Key for the new element.
 * @param i Associated info for the new element.
 *
 * After the call, the new element becomes the head of the list.
 *
 * Complexity: O(1)
 */
template <typename Key, typename Info>
void Sequence<Key, Info>::push_front(const Key& k, const Info& i) {
    Node* newNode = new Node(k, i);
    newNode->next = head;
    head = newNode;
}

/**
 * @brief Remove the first element of the list.
 * @return true if an element was removed, false if the list was already empty.
 *
 * Complexity: O(1)
 */
template <typename Key, typename Info>
bool Sequence<Key, Info>::pop_front() {
    if (is_empty()) return false;
    Node* temp = head;
    head = head->next;
    delete temp;
    return true;
}

/**
 * @brief Append a new element at the end of the list.
 * @param k Key for the new element.
 * @param i Associated info for the new element.
 *
 * If the list is empty the new element becomes the head. Requires traversal to find the tail.
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
void Sequence<Key, Info>::push_back(const Key& k, const Info& i) {
    Node* newNode = new Node(k, i);
    if (is_empty()) {
        head = newNode;
        return;
    }
    Node* current = head;
    while (current->next) {
        current = current->next;
    }
    current->next = newNode;
}

/**
 * @brief Remove the last element of the list.
 * @return true if an element was removed, false if the list was empty.
 *
 * For a single-element list the head is deleted and set to nullptr. Otherwise the list
 * is traversed to find the penultimate node which becomes the new tail.
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
bool Sequence<Key, Info>::pop_back() {
    if (is_empty()) return false;
    if (head->next == nullptr) {
        delete head;
        head = nullptr;
        return true;
    }
    Node* current = head;
    while (current->next && current->next->next) {
        current = current->next;
    }
    delete current->next;
    current->next = nullptr;
    return true;
}

/**
 * @brief Check whether the list is empty.
 * @return true if the list contains no elements, false otherwise.
 *
 * Complexity: O(1)
 */
template <typename Key, typename Info>
bool Sequence<Key, Info>::is_empty() const {
    return head == nullptr;
}

/**
 * @brief Remove all elements from the list.
 *
 * All nodes are deleted and the list becomes empty.
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
void Sequence<Key, Info>::clear() {
    while (!is_empty()) {
        pop_front();
    }
    head = nullptr;
}

/**
 * @brief Print the list contents to std::cout.
 *
 * Each node is printed as "(key, info) " in list order and a newline is printed at the end.
 * Requires that operator<< is defined for Key and Info types.
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
void Sequence<Key, Info>::print() const {
    Node* current = head;
    while (current) {
        std::cout << "(" << current->key << ", " << current->info << ") ";
        current = current->next;
    }
    std::cout << std::endl;
}

/**
 * @brief Assignment operator. Replaces contents with a deep copy of `other`.
 * @param other Sequence to copy from.
 * @return Reference to this Sequence.
 *
 * Self-assignment is handled. Existing contents are cleared before copying.
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
Sequence<Key, Info>& Sequence<Key, Info>::operator=(const Sequence& other) {
    if (this == &other) return *this;
    clear();
    Node* current = other.head;
    while (current) {
        push_back(current->key, current->info);
        current = current->next;
    }
    return *this;
}

/**
 * @brief Insert a new element at the specified zero-based position.
 * @param k Key for the new element.
 * @param i Associated info for the new element.
 * @param position Zero-based index where the new element should be inserted.
 *                 position == 0 inserts at the front.
 * @return true if insertion succeeded, false if position is out of bounds (position < 0
 *         or position greater than current size).
 *
 * If the list has fewer elements than position, the function returns false and no node
 * is inserted.
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
bool Sequence<Key, Info>::insert_at(const Key& k, const Info& i, int position)
{
    if (position < 0) return false;
    if (position == 0) {
        push_front(k, i);
        return true;
    }
    Node* newNode = new Node(k, i);
    Node* current = head;
    for (int idx = 0; idx < position - 1; ++idx) {
        if (!current) { // position is out of bounds
            delete newNode;
            return false;
        }
        current = current->next;
    }
    if (!current) { // position is out of bounds
        delete newNode;
        return false;
    }
    newNode->next = current->next;
    current->next = newNode;
    return true;
}

/**
 * @brief Remove the element at the specified zero-based position.
 * @param position Zero-based index of the element to remove.
 * @return true if removal succeeded, false if position is out of bounds or the list is empty.
 *
 * position == 0 removes the head element.
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
bool Sequence<Key, Info>::remove_at(int position) {
    if (position < 0 || is_empty()) return false;
    if (position == 0) {
        pop_front();
        return true;
    }
    Node* current = head;
    for (int idx = 0; idx < position - 1; ++idx) {
        if (!current->next) return false;
        current = current->next;
    }
    if (!current->next) return false;
    Node* temp = current->next;
    current->next = temp->next;
    delete temp;
    return true;
}

/**
 * @brief Compute the number of elements in the list.
 * @return The number of nodes currently stored in the sequence.
 *
 * This implementation traverses the entire list.
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
unsigned int Sequence<Key, Info>::size() const {
    int count = 0;
    Node* current = head;
    while (current) {
        ++count;
        current = current->next;
    }
    return count;
}

/**
 * @brief Retrieve the key stored at the given zero-based position.
 * @param position Zero-based index of the node whose key is requested.
 * @return A copy of the key at that position.
 * @throws std::out_of_range if position < 0 or position >= size().
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
Key Sequence<Key, Info>::get_key_at(int position) const {
    if (position < 0) throw std::out_of_range("Position out of range");
    Node* current = head;
    for (int idx = 0; idx < position; ++idx) {
        if (!current) throw std::out_of_range("Position out of range");
        current = current->next;
    }
    if (!current) throw std::out_of_range("Position out of range");
    return current->key;
}

/**
 * @brief Retrieve the info stored at the given zero-based position.
 * @param position Zero-based index of the node whose info is requested.
 * @return A copy of the info at that position.
 * @throws std::out_of_range if position < 0 or position >= size().
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
Info Sequence<Key, Info>::get_info_at(int position) const {
    if (position < 0) throw std::out_of_range("Position out of range");
    Node* current = head;
    for (int idx = 0; idx < position; ++idx) {
        if (!current) throw std::out_of_range("Position out of range");
        current = current->next;
    }
    if (!current) throw std::out_of_range("Position out of range");
    return current->info;
}

/**
 * @brief Reverse the order of elements in the list in-place.
 *
 * After the call, the head points to the former tail and the traversal order is reversed.
 *
 * Complexity: O(n), Additional memory: O(1)
 */
template <typename Key, typename Info>
void Sequence<Key, Info>::reverse() {
    Node* prev = nullptr;
    Node* current = head;
    Node* next = nullptr;
    while (current) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
}

/**
 * @brief Update the info field of the nth occurrence of a given key.
 * @param k Key to search for.
 * @param new_info New info value to assign to the matched node.
 * @param occurrence 1-based ordinal occurrence of the key to update (default: 1).
 * @throws std::invalid_argument if occurrence <= 0.
 *
 * The method traverses the list and counts matches of key `k`. When the count reaches
 * `occurrence`, that node's info is replaced with `new_info`. If fewer than `occurrence`
 * matches exist, no change is made.
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
void Sequence<Key, Info>::update_info(const Key& k, const Info& new_info, int occurrence) {
    if (occurrence <= 0) throw std::invalid_argument("Occurrence must be positive");
    Node* current = head;
    int count = 0;
    while (current) {
        if (current->key == k) {
            count++;
            if (count == occurrence) {
                current->info = new_info;
                return;
            }
        }
        current = current->next;
    }
}

/**
 * @brief Extract a subsequence starting at a given position with specified length.
 * @param start_pos Zero-based index to start the subsequence.
 * @param length Number of elements to include in the subsequence.
 * @param subseq Output Sequence to receive the extracted subsequence.
 * @throws std::out_of_range if start_pos < 0, length < 0, or the requested range exceeds list bounds.
 *
 * The method copies `length` elements starting from `start_pos` into `subseq`.
 * If the specified range is invalid, an exception is thrown.
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
void Sequence<Key, Info>::subsequence(int start_pos, int length, Sequence<Key, Info>& subseq) const {
    if (start_pos < 0 || length < 0) {
        throw std::out_of_range("Invalid start position or length");
    }
    Node* current = head;
    for (int idx = 0; idx < start_pos; ++idx) {
        if (!current) {
            throw std::out_of_range("Start position out of range");
        }
        current = current->next;
    }
    for (int len = 0; len < length; ++len) {
        if (!current) {
            throw std::out_of_range("Length exceeds list bounds");
        }
        subseq.push_back(current->key, current->info);
        current = current->next;
    }
}

/**
 * @brief Replace the key and info at the specified zero-based position.
 * @param position Zero-based index of the element to replace.
 * @param new_key New key to assign.
 * @param new_info New info to assign.
 * @throws std::out_of_range if position < 0 or position >= size().
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
void Sequence<Key, Info>::replace_at(int position, const Key& new_key, const Info& new_info) {
    if (position < 0) throw std::out_of_range("Position out of range");
    Node* current = head;
    for (int idx = 0; idx < position; ++idx) {
        if (!current) throw std::out_of_range("Position out of range");
        current = current->next;
    }
    if (!current) throw std::out_of_range("Position out of range");
    current->key = new_key;
    current->info = new_info;
}

/**
 * @brief Find the position of the nth occurrence of a given key.
 * @param k Key to search for.
 * @param occurrence 1-based ordinal occurrence to find.
 * @return Zero-based index of the found occurrence, or -1 if not found.
 * @throws std::invalid_argument if occurrence <= 0.
 *
 * The method traverses the list counting matches of key `k`. When the count reaches
 * `occurrence`, the current index is returned. If fewer than `occurrence` matches exist,
 * -1 is returned.
 *
 * Complexity: O(n)
 */
template <typename Key, typename Info>
int Sequence<Key, Info>::find_key_occurrence(const Key& k, int occurrence) const {
    if (occurrence <= 0) throw std::invalid_argument("Occurrence must be positive");
    Node* current = head;
    int count = 0;
    int index = 0;
    while (current) {
        if (current->key == k) {
            count++;
            if (count == occurrence) {
                return index;
            }
        }
        current = current->next;
        index++;
    }
    return -1; // Not found
}