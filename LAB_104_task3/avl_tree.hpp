#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cctype>

template <typename Key, typename Info>
class avl_tree {
private:
    struct node {
        Key key;
        Info info;
        node* left;
        node* right;
        int height;
        node(const Key& k, const Info& i) : key(k), info(i), left(nullptr), right(nullptr), height(1) {}
    };
    node* root;

    //functions that use node* or node as parameter or return type

    //print
    void print(node* n, int depth) const {
    if (!n) return;

    print(n->right, depth + 1);

    std::cout << std::string(depth * 4, ' ')
              << n->key << " : " << n->info << /*" " << n->height << */"\n";

    print(n->left, depth + 1);
    }

    //clear
    void clear(node* n){
        if(n != nullptr) {
            clear(n->left);
            clear(n->right);
            delete n;
        }
    }
    
    //height
    int height(node* n) const {
    return n ? n->height : 0;
    }

    //update height
    void update_height(node* n) {
    n->height = 1 + std::max(height(n->left), height(n->right));
    }

    //balance factor
    int balance_factor(node* n) const {
    return height(n->left) - height(n->right);
    }

    //RR
    node* rotate_right(node* y) {
    node* x = y->left;
    node* T2 = x->right;
    x->right = y;
    y->left = T2;
    update_height(y);
    update_height(x);
    return x;
    }

    //LL
    node* rotate_left(node* x) {
    node* y = x->right;
    node* T2 = y->left;
    y->left = x;
    x->right = T2;
    update_height(x);
    update_height(y);
    return y;
    }

    //rebalance
    node* rebalance(node* n) {
    update_height(n);
    int balance = balance_factor(n);
    // Left Left Case
    if (balance > 1 && balance_factor(n->left) >= 0)
        return rotate_right(n);
    // Right Right Case
    if (balance < -1 && balance_factor(n->right) <= 0)
        return rotate_left(n);
    // Left Right Case
    if (balance > 1 && balance_factor(n->left) < 0) {
        n->left = rotate_left(n->left);
        return rotate_right(n);
    }
    // Right Left Case
    if (balance < -1 && balance_factor(n->right) > 0) {
        n->right = rotate_right(n->right);
        return rotate_left(n);
    }
    return n;
    }

    //min node
    node* min_node(node* n) const {
    node* current = n;
    while (current->left != nullptr)
        current = current->left;
    return current;
    }

    //find
    node* find(const Key& key) {
    node* current = root;
    while (current != nullptr) {
        if (key == current->key)
            return current;
        else if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
    }

    //const find
    const node* find(const Key& key) const {
    const node* current = root;
    while (current != nullptr) {
        if (key == current->key)
            return current;
        else if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
    }

    //clone
    node* clone(node* n) const {
    if (n == nullptr)
        return nullptr;
    node* new_node = new node(n->key, n->info);
    new_node->left = clone(n->left);
    new_node->right = clone(n->right);
    new_node->height = n->height;
    return new_node;
    }
    
    //insert
    node* insert(node* n, const Key& key, const Info& info) {
    if (n == nullptr)
        return new node(key, info);
    if (key < n->key)
        n->left = insert(n->left, key, info);
    else if (key > n->key)
        n->right = insert(n->right, key, info);
    else {
        n->info = info; //update existing key
        return n;
    }
    return rebalance(n);
    }

    //remove
    node* remove(node* n, const Key& key) {
    if (n == nullptr)
        return n;
    if (key < n->key)
        n->left = remove(n->left, key);
    else if (key > n->key)
        n->right = remove(n->right, key);
    else {
        if ((n->left == nullptr) || (n->right == nullptr)) {
            node* temp = n->left ? n->left : n->right;
            if (temp == nullptr) {
                temp = n;
                n = nullptr;
            } else
                *n = *temp;
            delete temp;
        } else {
            node* temp = min_node(n->right);
            n->key = temp->key;
            n->info = temp->info;
            n->right = remove(n->right, temp->key);
        }
    }
    if (n == nullptr)
        return n;
    return rebalance(n);
    }

    //to_vector
    void to_vector(node* n, std::vector<std::pair<Key, Info>>& vec) const {
    if (n != nullptr) {
        to_vector(n->left, vec);
        vec.emplace_back(n->key, n->info);
        to_vector(n->right, vec);
    }
    }

public:
    avl_tree();
    avl_tree(const avl_tree& src);
    ~avl_tree();
    avl_tree& operator=(const avl_tree& src);
    Info& operator[](const Key& key); //permitting updates
    const Info& operator[](const Key& key) const; //indexing without updates
    bool search(const Key& key, Info& info) const;
    avl_tree& insert(const Key& key, const Info& info);
    avl_tree& remove(const Key& key);
    void clear();
    void print() const;
    void to_vector(std::vector<std::pair<Key, Info>>& vec) const;
    int size() const;
    bool empty() const;
};

template <typename Key, typename Info>
avl_tree<Key, Info>::avl_tree() : root(nullptr) {}

template <typename Key, typename Info>
avl_tree<Key, Info>::avl_tree(const avl_tree& src) : root(clone(src.root)) {}

template <typename Key, typename Info>
avl_tree<Key, Info>::~avl_tree() {
    clear(root);
}

template <typename Key, typename Info>
avl_tree<Key, Info>& avl_tree<Key, Info>::operator=(const avl_tree& src) {
    if (this != &src) {
        clear();
        root = clone(src.root);
    }
    return *this;
}

template <typename Key, typename Info>
Info& avl_tree<Key, Info>::operator[](const Key& key) {
    node* n = find(key);
    if (!n) {
        root = insert(root, key, Info{});
        n = find(key);
    }
    return n->info;
}


template <typename Key, typename Info>
const Info& avl_tree<Key, Info>::operator[](const Key& key) const {
    const node* result = find(key); //type node is not revealed outside the interface, so encapsulation is preserved
    if (result) {
        return result->info;
    }
    static Info dummy;
    return dummy;
}

template <typename Key, typename Info>
bool avl_tree<Key, Info>::search(const Key& key, Info& info) const {
    const node* result = find(key);
    if (result) {
        info = result->info;
        return true;
    }
    return false;
}

template <typename Key, typename Info>
avl_tree<Key, Info>& avl_tree<Key, Info>::insert(const Key& key, const Info& info) {
    root = insert(root, key, info);
    return *this;
}

template <typename Key, typename Info>
avl_tree<Key, Info>& avl_tree<Key, Info>::remove(const Key& key) {
    root = remove(root, key);
    return *this;
}

template <typename Key, typename Info>
void avl_tree<Key, Info>::clear() {
    clear(root);
    root = nullptr;
}

template <typename Key, typename Info>
void avl_tree<Key, Info>::print() const {
    print(root, 0);
}

template <typename Key, typename Info>
void avl_tree<Key, Info>::to_vector(std::vector<std::pair<Key, Info>>& vec) const {
    to_vector(root, vec);
}

template <typename Key, typename Info>
int avl_tree<Key, Info>::size() const {
    std::vector<std::pair<Key, Info>> vec;
    to_vector(vec);
    return static_cast<int>(vec.size());
}

template <typename Key, typename Info>
bool avl_tree<Key, Info>::empty() const {
    return root == nullptr;
}

template <typename Key, typename Info>
std::vector<std::pair<Key, Info>> maxinfo_selector(const avl_tree<Key, Info>& tree, unsigned cnt) {
    std::vector<std::pair<Key, Info>> v;
    tree.to_vector(v);

    std::sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.second > b.second; });

    if (cnt < v.size())
        v.resize(cnt);

    return v;
}

avl_tree<std::string, int> count_words(std::istream& is) {
    avl_tree<std::string, int> word_count;
    std::string word;

    while (is >> word) {
        std::string cleaned_word;
        for (char c : word) {
            if (std::isalnum(static_cast<unsigned char>(c))) {
                cleaned_word += std::tolower(static_cast<unsigned char>(c));
            }
        }

        if (!cleaned_word.empty()) {
            word_count[cleaned_word]++;
        }
    }

    return word_count;
}