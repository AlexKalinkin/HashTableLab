#include "hash_table.hpp"
#include <iostream>

unsigned HashTable::hash_fun(const int prime_number, const Key& k) const {
    size_t len = k.length();
    unsigned hash = 0;
    for (int i = 0; i < len; i++) {
        hash += (hash * prime_number) + k[i];
    }

    return hash % capacity_;
}

unsigned HashTable::hash_function1(const Key& k) const {
    unsigned hash = hash_fun(first_prime, k);
    return hash % capacity_;
}

unsigned HashTable::hash_function2(const Key& k) const {
    unsigned hash = hash_fun(second_prime, k);
    if (hash % 2 != 0) return hash;
    return hash + 1;
}

int HashTable::search(const Key& k) const {
    unsigned hash1 = hash_function1(k);
    unsigned hash2 = hash_function2(k);
    for (int i = 0; i < capacity_; i++) {
        if (bucket[hash1] != nullptr) {
            if (!bucket[hash1]->deleted && bucket[hash1]->key == k) return hash1;
        }
        else return -1;
        hash1 = (hash1 + hash2) % capacity_;
    }
    return -1;
}

void HashTable::add_bucket(const Key& k, const Value& v, unsigned hash1) {
    Bucket* a = new Bucket(k, v);
    bucket[hash1] = a;
    size_++;
}

void HashTable::resize() {
    capacity_ *= 2;
    Bucket** new_bucket = new Bucket * [capacity_]();

    size_ = 0;
    std::swap(bucket, new_bucket);
    for (int i = 0; i < capacity_ / 2; i++) {
        if (new_bucket[i] != nullptr && new_bucket[i]->deleted == false) insert(new_bucket[i]->key, new_bucket[i]->value);
    }

    for (int i = 0; i < capacity_ / 2; i++)
        if (new_bucket[i]) delete new_bucket[i];

    delete[] new_bucket;
}

HashTable::HashTable() : capacity_(default_capacity), bucket(new Bucket* [default_capacity]()) {}

HashTable::~HashTable() {
    for (int i = 0; i < capacity_; i++) {
        if (bucket[i] != nullptr) delete bucket[i];
    }
    delete[] bucket;
}

HashTable& HashTable::operator=(const HashTable& b) {
    if (this != &b) {
        for (int i = 0; i < capacity_; i++) {
            if (bucket[i] != nullptr) delete bucket[i];
        }
        delete[] bucket;

        size_ = b.size_;
        capacity_ = b.capacity_;
        bucket = new Bucket* [b.capacity_]();
        for (int i = 0; i < capacity_; i++) {
            if (b.bucket[i] != nullptr) {
                bucket[i] = new Bucket(b.bucket[i]->key, b.bucket[i]->value, b.bucket[i]->deleted);
            }
        }
    }
    return *this;
}

HashTable::HashTable(const HashTable& b) : capacity_(b.capacity_), size_(b.size_), bucket(new Bucket* [b.capacity_]()) {
    for (int i = 0; i < capacity_; i++) {
        if (b.bucket[i] != nullptr) {
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
        if (bucket[i] != nullptr) {
            delete bucket[i];
            bucket[i] = nullptr;
        }
    }
    size_ = 0;
}

bool HashTable::erase(const Key& k) {
    unsigned hash1 = hash_function1(k);
    unsigned hash2 = hash_function2(k);
    for (int i = 0; i < capacity_; i++) {
        if (bucket[hash1] != nullptr) {
            if (bucket[hash1]->key == k) {
                bucket[hash1]->deleted = true;
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

    unsigned hash1 = hash_function1(k);
    unsigned hash2 = hash_function2(k);

    for (int i = 0; i < capacity_; i++) {
        if (bucket[hash1] == nullptr) {
            add_bucket(k, v, hash1);
            return true;
        }

        else if (bucket[hash1]->deleted) {
            delete bucket[hash1];
            add_bucket(k, v, hash1);
            bucket[hash1]->deleted = false;
            return true;
        }

        else if (k == bucket[hash1]->key) {
            bucket[hash1]->value = v;
            return false;
        }
        hash1 = (hash1 + hash2) % capacity_;
    }
}

bool HashTable::contains(const Key& k) const {
    int index = search(k);
    return index != -1;
}

Value& HashTable::operator[](const Key& k) {
    int index = search(k);
    if (index == -1) {
        Value a("", 0);
        unsigned hash1 = hash_function1(k);
        unsigned hash2 = hash_function2(k);

        for (int i = 0; i < capacity_; i++) {
            if (bucket[hash1] == nullptr) add_bucket(k, a, hash1);

            else if (bucket[hash1]->deleted) {
                delete bucket[hash1];
                add_bucket(k, a, hash1);
                bucket[hash1]->deleted = false;
            }
            hash1 = (hash1 + hash2) % capacity_;

            return (bucket[hash1]->value);
        }
    }

    return (bucket[index]->value);
}
   
Value& HashTable::at(const Key& k) {
    int index = search(k);
    if (index == -1) throw std::runtime_error("No value for this key");

    return (bucket[index]->value);
}

const Value& HashTable::at(const Key& k) const {
    int index = search(k);
    if (index == -1) throw std::runtime_error("No value for this key");

    return const_cast<Value&>(bucket[index]->value);
}

size_t HashTable::size() const {
    return size_;
}

bool HashTable::empty() const {
    return size_ == 0;
}

bool operator!=(Value& a, Value& b) {
    if (a.name == b.name && a.age == b.age) return false;
    return true;
}

bool operator==(const HashTable& a, const HashTable& b) {
    if (a.size_ != b.size_) return false;

    for (int i = 0; i < a.capacity_; i++) {
        if (a.bucket[i] == nullptr || a.bucket[i]->deleted == true) continue;
        int index = b.search(a.bucket[i]->key);
        if (index == -1) return false;
        if (a.bucket[i]->value != b.bucket[index]->value) return false;
    }
    return true;
}

bool operator!=(const HashTable& a, const HashTable& b) {
    return !(a == b);
}