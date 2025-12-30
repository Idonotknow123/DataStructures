#pragma once 
#include <iostream>

using namespace std;

template <typename Key, typename Info>
class bi_ring {
    private:
        struct Node {
            Key key;
            Info info;
            Node* next;
            Node* prev;

            //Node(const Key& value) : key(value), next(nullptr), prev(nullptr) {}
            Node(const Key& k, const Info& i) : key(k), info(i), next(this), prev(this) {}
        };
        int version;
        Node* any;
    public:
    class iterator {
        friend class bi_ring;
        private: 
            Node* node;
            int version;
        public:
            iterator(Node* n, int v) : node(n), version(v) {}
            Key& key() { return node->key; }
            Info& info() { return node->info; }
            iterator& operator++() {
                node = node->next;
                return *this;
            }
            iterator operator++(int) {
                iterator temp = *this;
                node = node->next;
                return temp;
            }
            iterator& operator--() {
                node = node->prev;
                return *this;
            }
            iterator operator--(int) {
                iterator temp = *this;
                node = node->prev;
                return temp;
            }
            bool operator!=(const iterator& other) const {
                return node != other.node;
            }
            iterator& operator=(const iterator& other) {
                node = other.node;
                version = other.version;
                return *this;
            }
            iterator(const iterator& other) : node(other.node), version(other.version) {}
            iterator& operator*() {
                return *this;
            }
            iterator* operator->() {
                return this;
            }
            
    };
    class const_iterator {
            friend class bi_ring;

        private:
            const Node* node;
            int version;

        public:
            const_iterator(const Node* n, int v) : node(n), version(v) {}
            const_iterator(const const_iterator& other) 
                : node(other.node), version(other.version) {}

            // umożliwia konwersję iterator -> const_iterator
            const_iterator(const iterator& other)
                : node(other.node), version(other.version) {}

            const Key& key() const { return node->key; }
            const Info& info() const { return node->info; }

            const_iterator& operator++() {
                node = node->next;
                return *this;
            }
            const_iterator operator++(int) {
                const_iterator temp = *this;
                node = node->next;
                return temp;
            }
            const_iterator& operator--() {
                node = node->prev;
                return *this;
            }
            const_iterator operator--(int) {
                const_iterator temp = *this;
                node = node->prev;
                return temp;
            }

            bool operator!=(const const_iterator& other) const {
                return node != other.node;
            }

            const_iterator& operator=(const const_iterator& other) {
                node = other.node;
                version = other.version;
                return *this;
            }

            // dereference returns const iterator to mimic your structure
            const_iterator operator*() const {
                return *this;
            }

            const_iterator* operator->() {
                return this;
            }
        };

    
        //- - - - - METHODS - - - - -
        bi_ring() : any(nullptr), version(0) {};

        bi_ring(const bi_ring& other) : any(nullptr), version(0) {
            if (!other.any) return;
            Node* current = other.any;
            do {
                this->push_back(current->key, current->info);
                current = current->next;
            } while (current != other.any);
        }

        ~bi_ring() {
            if (!any) return;
            Node* current = any->next;
            while (current != any) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            delete any;
        }

        bi_ring& operator=(const bi_ring& other) {
            if (this == &other) return *this;
            this->~bi_ring();
            any = nullptr;
            version = 0;
            if (!other.any) return *this;
            Node* current = other.any;
            do {
                this->push_back(current->key, current->info);
                current = current->next;
            } while (current != other.any);
            return *this;
        }

        iterator push_front(const Key& key, const Info& info) {
            Node* newNode = new Node(key, info);
            if (!any) {
                newNode->next = newNode;
                newNode->prev = newNode;
                any = newNode;
            } else {
                Node* tail = any->prev;
                newNode->next = any;
                newNode->prev = tail;
                tail->next = newNode;
                any->prev = newNode;
                any = newNode;
            }
            version++;
            return iterator(newNode, version);
        }

        iterator pop_front() {
            if (!any) return iterator(nullptr, version);
            Node* toDelete = any;
            if (any->next == any) {
                any = nullptr;
            } else {
                Node* tail = any->prev;
                any = any->next;
                tail->next = any;
                any->prev = tail;
            }
            delete toDelete;
            version++;
            return iterator(any, version);
        }

        iterator push_back(const Key& key, const Info& info) {
            Node* newNode = new Node(key, info);
            if (!any) {
                any = newNode;
            } else {
                Node* tail = any->prev;
                newNode->next = any;
                newNode->prev = tail;
                tail->next = newNode;
                any->prev = newNode;
            }
            version++;
            return iterator(newNode, version);
        }

        iterator pop_back() {
            if (!any) return iterator(nullptr, version);
            Node* tail = any->prev;
            if (tail == any) {
                any = nullptr;
            } else {
                Node* newTail = tail->prev;
                newTail->next = any;
                any->prev = newTail;
            }
            delete tail;
            version++;
            return iterator(any, version);
        }

        iterator insert(iterator position, const Key& key, const Info& info) {
            if (position.version != version) {
                throw runtime_error("Iterator version mismatch");
            }
            Node* newNode = new Node(key, info);
            Node* posNode = position.node;
            Node* prevNode = posNode->prev;
            newNode->next = posNode;
            newNode->prev = prevNode;
            prevNode->next = newNode;
            posNode->prev = newNode;
            version++;
            return iterator(newNode, version);
        }

        iterator erase(iterator position) {
            if (position.version != version || !any) {
                throw runtime_error("Iterator version mismatch or empty ring");
            }
            Node* toDelete = position.node;
            if (toDelete->next == toDelete) {
                any = nullptr;
            } else {
                Node* prevNode = toDelete->prev;
                Node* nextNode = toDelete->next;
                prevNode->next = nextNode;
                nextNode->prev = prevNode;
                if (toDelete == any) {
                    any = nextNode;
                }
            }
            delete toDelete;
            version++;
            return iterator(any, version);
        }

        iterator begin() const { 
            return iterator(any, version); 
        }

        iterator end() const { 
            return iterator(any, version); 
        } //let's agree, that end() points to the same element as begin()

        void print() {
            if (!any) {
                cout << "Ring is empty." << endl;
                return;
            }
            Node* current = any;
            do {
                cout << current->key << ": " << current->info << endl;
                current = current->next;
            } while (current != any);
        }

        void change_any(int steps) {
            if (!any) return;
            Node* current = any;
            if (steps > 0) {
                for (int i = 0; i < steps; ++i) {
                    current = current->next;
                }
            } else {
                for (int i = 0; i < -steps; ++i) {
                    current = current->prev;
                }
            }
            any = current;
        }

        /*int find(const Key& key) {
            if (!any) return -1;
            Node* current = any;
            int index = 0;
            do {
                if (current->key == key) {
                    return index;
                }
                current = current->next;
                index++;
            } while (current != any);
            return -1;
        }*/

        bool is_empty() const {
            return any == nullptr;
        }

        const_iterator find(const Key& key) const {
            if (!any) return const_iterator(nullptr, version);
            Node* current = any;
            do {
                if (current->key == key) {
                    return const_iterator(current, version);
                }
                current = current->next;
            } while (current != any);
            return const_iterator(nullptr, version);
        }

        bool is_key_in_ring(const Key& key) const {
            if (!any) return false;
            Node* current = any;
            do {
                if (current->key == key) {
                    return true;
                }
                current = current->next;
            } while (current != any);
            return false;
        }
};

template <typename Key, typename Info>
bi_ring<Key, Info> join(const bi_ring<Key, Info>& first, const bi_ring<Key, Info>& second)
{
    bi_ring<Key, Info> result;

    if (first.is_empty() && second.is_empty())
        return result;

    if (!first.is_empty()) {
        auto it1 = first.begin();
        do {
            Key k = it1.key();
            Info v = it1.info();

            if (second.is_key_in_ring(k)) {
                auto it2 = second.find(k);
                v = v + it2.info();
            }

            result.push_back(k, v);
            it1++;
        } while (it1.key() != first.begin().key());
    }

    if (!second.is_empty()) {
        auto it2 = second.begin();
        do {
            Key k = it2.key();
            if (!first.is_key_in_ring(k)) {
                result.push_back(k, it2.info());
            }
            it2++;
        } while (it2.key() != second.begin().key());
    }

    return result;
}

template <typename Key, typename Info>
bi_ring<Key, Info> shuffle(const bi_ring<Key, Info>& first, unsigned int fcnt, const bi_ring<Key, Info>& second, unsigned int scnt, unsigned int reps) {
     bi_ring<Key, Info> result;
    //both empty
        if (first.is_empty() && second.is_empty())
        return result;
    
    // first not empty
    if (second.is_empty()) {
        if (first.is_empty() || fcnt == 0 || reps == 0)
            return result;
            
        auto it1 = first.begin();
        for (unsigned int r = 0; r < reps; r++) {
            for (unsigned int i = 0; i < fcnt; i++) {
                result.push_back(it1.key(), it1.info());
                it1++;
            }
        }
        return result;
    }
    
    // second not empty
    if (first.is_empty()) {
        if (second.is_empty() || scnt == 0 || reps == 0)
            return result;
            
        auto it2 = second.begin();
        for (unsigned int r = 0; r < reps; r++) {
            for (unsigned int i = 0; i < scnt; i++) {
                result.push_back(it2.key(), it2.info());
                it2++;
            }
        }
        return result;
    }

    // none empty
    auto it1 = first.is_empty() ? typename bi_ring<Key,Info>::iterator(nullptr, 0) : first.begin();
    auto it2 = second.is_empty() ? typename bi_ring<Key,Info>::iterator(nullptr, 0) : second.begin();

    for (unsigned int r = 0; r < reps; r++) {

        for (unsigned int i = 0; i < fcnt /*&& !first.is_empty()*/; i++) { //purposefully no check for first.is_empty() here, because it is a ring
            result.push_back(it1.key(), it1.info());
            it1++;
        }

        for (unsigned int i = 0; i < scnt /*&& !second.is_empty()*/; i++) { //purposefully no check for second.is_empty() here, because it is a ring
            result.push_back(it2.key(), it2.info());
            it2++;
        }
    }

    return result;
}