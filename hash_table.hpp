﻿#include <iostream>
typedef std::string Key;

struct Value {
    explicit Value(const std::string& n, unsigned a = 0) : name(n), age(a) {}
    std::string name;
    unsigned age;
};

class HashTable {
public:
    // Creates the table 
    HashTable();

    // Deletes the table
    ~HashTable();

    // Assigns one table to another table
    HashTable& operator=(const HashTable& b);

    // Copies table b
    HashTable(const HashTable& b);

    // Changes buckets with the same index of two tables 
    void swap(HashTable& b);

    // Сlears buckets in the table 
    void clear();

    // Retruns true if found a bucket_ by key and deleted it
    // Retruns false else 
    bool erase(const Key& k);

    // Returns true if inserted v in the table 
    // Returns false if there is a bucket_ with the same key
    bool insert(const Key& k, const Value& v);

    // Returns true if the table contains a bucket_ with such a key
    // Returns false if the table doesn't contain a bucket_ with such a key
    bool contains(const Key& k) const;

    // Returns a link to value by key, but if there is no value for this key, a new bucket_ is created with name = "" and age = 0
    Value& operator[](const Key& k);

    // Returns a link to value by key
    // If not found throws runtime_error
    Value& at(const Key& k);
    const Value& at(const Key& k) const;

    // Returns the number of non-empty buckets in the table
    size_t size() const;

    // Returns true if there are no initialized buckets in the table
    // Returns false if the table has at least one initialized bucket_
    bool empty() const;

    // Returns true if tables have the same buckets 
    // Otherwise returns false
    friend bool operator==(const HashTable& a, const HashTable& b);

    // Returns false if tables have the same buckets 
    // Otherwise returns true
    friend bool operator!=(const HashTable& a, const HashTable& b);

private:
    int capacity_;
    int size_;
    const static int DEFAULT_CAPACITY = 4;
    const static int FIRST_PRIME = 7;
    const static int SECOND_PRIME = 13;

    struct Bucket {
        Bucket(const Key& k, const Value& v, bool d = false) : key(k), value(v), deleted(d) {}
        Key key;
        Value value;
        bool deleted;
    };
    Bucket** bucket_;

    unsigned hash_fun(int prime_number, const Key& key) const;
    unsigned hash_function1(const Key& key) const;
    unsigned hash_function2(const Key& key) const;

    void resize();

    int search(const Key& key) const;

    void add_bucket(const Key& k, const Value& v, unsigned hash1);
};  