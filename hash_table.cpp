#include "hash_table.hpp"
#include <iostream>

int HashTable::hash_function1(const Key& k) const {
    size_t len = k.length();
    unsigned int hash = 0;
    for (int i = 0; i < len; i++) {
        hash += (hash * first_prime) + k[i];
    }
    return hash % capacity_;
}

int HashTable::hash_function2(const Key& k) const {
    size_t len = k.length();
    unsigned hash = 0;
    for (int i = 0; i < len; i++) {
        hash += (hash * second_prime) + k[i];
    }
    hash = hash % capacity_;

    if (hash % 2 != 0) return hash;
    return hash + 1;
}

int HashTable::search(const Key& k) const {
    int hash1 = hash_function1(k);
    int hash2 = hash_function2(k);
    for (int i = 0; i < capacity_; i++) {
        if (bucket[hash1] != nullptr) {
            if (bucket[hash1]->deleted == false && bucket[hash1]->key == k) return hash1;
        }
        else return -1;
        hash1 = (hash1 + hash2) % capacity_;
    }
    return -1;
}

void HashTable::resize() {
    capacity_ *= 2;
    const Bucket** new_bucket = new const Bucket * [capacity_]();

    size_ = 0;
    std::swap(bucket, new_bucket);
    for (int i = 0; i < capacity_ / 2; i++) {
        if (new_bucket[i] != nullptr && new_bucket[i]->deleted == false) insert(new_bucket[i]->key, new_bucket[i]->value);
    }

    for (int i = 0; i < capacity_ / 2; i++)
        if (new_bucket[i]) delete new_bucket[i];

    delete[] new_bucket;
}

HashTable::HashTable() : capacity_(default_capacity), size_(0), bucket(new const Bucket* [default_capacity]()) {}

HashTable::~HashTable() {
    for (int i = 0; i < capacity_; i++) {
        if (bucket[i] != nullptr) delete bucket[i];
    }
    delete[] bucket;
}

HashTable& HashTable::operator=(const HashTable& b) {
    if (this != &b) {
        this->~HashTable();
        size_ = b.size_;
        capacity_ = b.capacity_;
        bucket = new const Bucket * [b.capacity_]();
        for (int i = 0; i < capacity_; i++) {
            if (b.bucket[i] != nullptr) {
                if (bucket[i] != nullptr) delete bucket[i];
                bucket[i] = new Bucket(b.bucket[i]->key, b.bucket[i]->value, b.bucket[i]->deleted);
            }
        }
    }
    return *this;
}

HashTable::HashTable(const HashTable& b) : capacity_(b.capacity_), size_(b.size_), bucket(new const Bucket* [b.capacity_]()) {
    for (int i = 0; i < capacity_; i++) {
        if (b.bucket[i] != nullptr) {
            if (bucket[i] != nullptr) delete bucket[i];

            bucket[i] = new Bucket(b.bucket[i]->key, b.bucket[i]->value, b.bucket[i]->deleted);

        }
    }
}

void HashTable::swap(HashTable& b) {
    std::swap(capacity_, b.capacity_);
    std::swap(size_, b.size_);
    std::swap(bucket, b.bucket);
}

void HashTable::clear() {
    for (int i = 0; i < capacity_; i++) {
        if (bucket[i] != nullptr) bucket[i] = nullptr;
    }
    size_ = 0;
}

bool HashTable::erase(const Key& k) {
    int hash1 = hash_function1(k);
    int hash2 = hash_function2(k);
    for (int i = 0; i < capacity_; i++) {
        if (bucket[hash1] != nullptr) {
            if (bucket[hash1]->key == k) {
                const_cast<bool&>(bucket[hash1]->deleted) = true;
                size_--;
                return true;

            }
        }
        else return false;
        hash1 = (hash1 + hash2) % capacity_;
    }
}
bool HashTable::insert(const Key& k, const Value& v) {
    if (size_ >= capacity_ * 0.75) resize();
    int hash1 = hash_function1(k);
    int hash2 = hash_function2(k);

    for (int i = 0; i < capacity_; i++) {
        if (bucket[hash1] == nullptr) {
            const Bucket* a = new const Bucket(k, v);
            bucket[hash1] = a;
            const_cast<bool&>(bucket[hash1]->deleted) = false;
            size_++;
            return true;
        }

        else if (bucket[hash1]->deleted == true) {
            const_cast<bool&>(bucket[hash1]->deleted) = false;
            const_cast<Key&>(bucket[hash1]->key) = k;
            const_cast<Value&>(bucket[hash1]->value) = v;
            size_++;
            return true;
        }

        else if (k == bucket[hash1]->key) {
            const_cast<Key&>(bucket[hash1]->key) = k;
            const_cast<Value&>(bucket[hash1]->value) = v;
            return false;
        }
        hash1 = (hash1 + hash2) % capacity_;
    }
}

bool HashTable::contains(const Key& k) const {
    int index = search(k);
    if (index == -1) return false;
    return true;
}

Value& HashTable::operator[](const Key& k) {
    int index = search(k);
    if (index == -1) {
        static Value a("", 0);
        insert(k, a);
        return const_cast<Value&>(a);

    }
    return const_cast<Value&>(bucket[index]->value);
}

Value& HashTable::at(const Key& k) {
    int index = search(k);
    if (index == -1) throw "No value for this key";

    return const_cast<Value&>(bucket[index]->value);
}

const Value& HashTable::at(const Key& k) const {
    int index = search(k);
    if (index == -1) throw "No value for this key";

    return (bucket[index]->value);
}

size_t HashTable::size() const {
    return size_;
}

bool HashTable::empty() const {
    if (size_ == 0) return true;

    return false;
}

bool operator==(const HashTable& a, const HashTable& b) {
    if (a.size_ != b.size_) return false;

    for (int i = 0; i < a.capacity_; i++) {
        if (a.bucket[i] == nullptr) continue;
        if (!b.contains(a.bucket[i]->key)) return false;
    }
    return true;
}

bool operator!=(const HashTable& a, const HashTable& b) {
    return !(a == b);
}