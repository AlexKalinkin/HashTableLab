#include "hash_table.hpp"
#include <cassert> 
unsigned HashTable::hash_fun(const int prime_number, const Key& k) const {
    size_t len = k.length();
    unsigned hash = 0;
    for (int i = 0; i < len; i++) {
        hash += (hash * prime_number) + k[i];
    }

    return hash % capacity_;
}

unsigned HashTable::hash_function1(const Key& k) const {
    return hash_fun(FIRST_PRIME, k);
}

unsigned HashTable::hash_function2(const Key& k) const {
    unsigned hash = hash_fun(SECOND_PRIME, k);
    if (hash % 2 != 0) return hash;
    // CR: we have a problem when hash = capacity - 1
    
    // Ans: No problem because capacity is an even number
    return hash + 1;
}

int HashTable::search(const Key& k) const {
    unsigned hash1 = hash_function1(k);
    unsigned hash2 = hash_function2(k);
    for (int i = 0; i < capacity_; i++) {
        if (bucket_[hash1] != nullptr) {
            if (!bucket_[hash1]->deleted && bucket_[hash1]->key == k) return int(hash1);
        }
        else return -1;
        hash1 = (hash1 + hash2) % capacity_;    
    }
    return -1;
}

void HashTable::add_bucket(const Key& k, const Value& v, unsigned hash1) {
    Bucket* a = new Bucket(k, v);
    bucket_[hash1] = a;
    size_++;
}

void HashTable::resize() {
    capacity_ *= 2;
    Bucket** new_bucket = new Bucket * [capacity_]();

    size_ = 0;
    std::swap(bucket_, new_bucket);
    for (int i = 0; i < capacity_ / 2; i++) {
        if (new_bucket[i] != nullptr && !new_bucket[i]->deleted) insert(new_bucket[i]->key, new_bucket[i]->value);
        if (new_bucket[i]) delete new_bucket[i];

    }

    delete[] new_bucket;
}

HashTable::HashTable() : capacity_(DEFAULT_CAPACITY), size_(), bucket_(new Bucket* [DEFAULT_CAPACITY]()) {}

HashTable::~HashTable() {
    for (int i = 0; i < capacity_; i++) {
        if (bucket_[i] != nullptr) delete bucket_[i];
    }
    delete[] bucket_;
}

HashTable& HashTable::operator=(const HashTable& b) {
    if (this != &b) {
        for (int i = 0; i < capacity_; i++) {
            if (bucket_[i] != nullptr) delete bucket_[i];
        }
        delete[] bucket_;

        size_ = b.size_;
        capacity_ = b.capacity_;
        bucket_ = new Bucket* [b.capacity_]();
        for (int i = 0; i < capacity_; i++) {
            if (b.bucket_[i] != nullptr) {
                bucket_[i] = new Bucket(b.bucket_[i]->key, b.bucket_[i]->value, b.bucket_[i]->deleted);
            }
        }
    }
    return *this;
}

HashTable::HashTable(const HashTable& b) : capacity_(b.capacity_), size_(b.size_), bucket_(new Bucket* [b.capacity_]()) {
    for (int i = 0; i < capacity_; i++) {
        if (b.bucket_[i] != nullptr) {
            bucket_[i] = new Bucket(b.bucket_[i]->key, b.bucket_[i]->value, b.bucket_[i]->deleted);

        }
    }
}

void HashTable::swap(HashTable& b) {
    std::swap(capacity_, b.capacity_);
    std::swap(size_, b.size_);
    std::swap(bucket_, b.bucket_);
}

void HashTable::clear() {
    for (int i = 0; i < capacity_; i++) {
        if (bucket_[i] != nullptr) {
            delete bucket_[i];
            bucket_[i] = nullptr;
        }
    }
    size_ = 0;
}

bool HashTable::erase(const Key& k) {
    unsigned hash1 = hash_function1(k);
    unsigned hash2 = hash_function2(k);
    for (int i = 0; i < capacity_; i++) {
        if (bucket_[hash1] != nullptr) {
            if (bucket_[hash1]->key == k) {
                bucket_[hash1]->deleted = true;
                size_--;
                return true;
            }
        }
        else return false;
        hash1 = (hash1 + hash2) % capacity_;
    }
    assert(false);
}

bool HashTable::insert(const Key& k, const Value& v) {
    if (size_ >= capacity_ * 0.75) resize();

    unsigned hash1 = hash_function1(k);
    unsigned hash2 = hash_function2(k);

    int best_bucket = -1;

    for (int i = 0; i < capacity_; i++) {
        if (bucket_[hash1] == nullptr) { 
            add_bucket(k, v, best_bucket == -1 ? hash1 : best_bucket);
            return true;
        }

        else if (bucket_[hash1]->key == k) { 
            bucket_[hash1]->value = v;
            return false;
        }
        else if (bucket_[hash1]->deleted == true && best_bucket == -1) best_bucket = hash1;
        hash1 = (hash1 + hash2) % capacity_;
    }
    assert(false);
}

bool HashTable::contains(const Key& k) const {
    int index = search(k);
    return index != -1;
}

Value& HashTable::operator[](const Key& k) {
    int index = search(k);
    if (index == -1) {
        Value v("", 0);
        unsigned hash1 = hash_function1(k);

        int best_bucket = -1;

        for (int i = 0; i < capacity_; i++) {

            if (bucket_[hash1] == nullptr) {
                add_bucket(k, v, best_bucket == -1 ? hash1 : best_bucket);
                return (bucket_[hash1]->value);
            }

            else if (bucket_[hash1]->deleted == true && best_bucket == -1) best_bucket = hash1;
        }
    }

    return (bucket_[index]->value);
}
   
Value& HashTable::at(const Key& k) {
    int index = search(k);
    if (index == -1) throw std::runtime_error("No value for this key");

    return (bucket_[index]->value);
}

const Value& HashTable::at(const Key& k) const {
    int index = search(k);
    if (index == -1) throw std::runtime_error("No value for this key");

    return const_cast<Value&>(bucket_[index]->value);
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
        if (a.bucket_[i] == nullptr || a.bucket_[i]->deleted) continue;
        int index = b.search(a.bucket_[i]->key);
        if (index == -1) return false;
        if (a.bucket_[i]->value != b.bucket_[index]->value) return false;
    }
    return true;
}

bool operator!=(const HashTable& a, const HashTable& b) {
    return !(a == b);
}