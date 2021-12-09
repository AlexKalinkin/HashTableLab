#include <gtest/gtest.h>
#include "hash_table.hpp"

#include <ctime>
#include <iostream>

std::string rand_string(const int len) {
    char alphabet[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string rand_string;

    for (int i = 0; i < len; i++) {
        rand_string += alphabet[rand() % (sizeof(alphabet) - 1)];
    }

    return rand_string;
}

TEST(HashTableTest, CheckBigInsert) {
    HashTable table;
    Value a("Alex", 20);
    Value b("Bob", 40);
    for (int i = 0; i < 100000; i++) {
        table.insert(rand_string(10), a);
    }
    EXPECT_EQ(table.size(), 100000);
}

TEST(HashTableTest, CheckInsertEqualKeys) {
    HashTable table;
    Value a("Alex", 20);
    Value b("Bob", 40);
    table.insert("asdasd", a);
    table.insert("asdasd", b);
    EXPECT_EQ(table.size(), 1);
}

TEST(HashTableTest, CheckInsertAndErase) {
    HashTable table;
    Value a("Alex", 20);
    table.insert("asdasd", a);
    table.erase("asdasd");
    EXPECT_EQ(table.size(), 0);
}


TEST(HashTableTest, CheckContains) {
    HashTable table;
    Value a("Alex", 20);
    EXPECT_FALSE(table.contains("ad"));
    table.insert("minca", a);
    EXPECT_TRUE(table.contains("minca"));
}

TEST(HashTableTest, CheckSwap) {
    HashTable table1;
    HashTable table2;
    Value a("Alex", 20);
    Value b("Bob", 40);
    table1.insert("adqsdasd", a);
    table1.insert("as", b);
    table1.insert("asdadasd", a);
    table1.insert("asddaasd", b);
    table2.insert("aqsdaasd", a);
    table2.insert("qasdasd", b);
    table1.swap(table2);
    EXPECT_TRUE(table1 != table2);

}

TEST(HashTableTest, CheckSwapEmptyTables) {
    HashTable table1;
    HashTable table2;
    table1.swap(table2);
    EXPECT_TRUE(table1 == table2);
}
TEST(HashTableTest, CheckAssignEmptyTables) {
    HashTable table1;
    HashTable table2;
    table1 = table2;
    EXPECT_TRUE(table1 == table2);
}

TEST(HashTableTest, CheckAssing) {
    HashTable table1;
    HashTable table2;
    Value a("Alex", 20);
    Value b("Bob", 40);
    table1.insert("adqsdasd", a);
    table1.insert("as", b);
    table1.insert("asdadasd", a);
    table1.insert("asddaasd", b);
    table2.insert("aqsdaasd", a);
    table2.insert("qasdasd", b);
    table1 = table2;
    EXPECT_EQ(table1.size(), 2);
    EXPECT_TRUE(table1 == table2);
}

TEST(HashTableTest, CheckErasre) {
    HashTable table1;
    Value a("Alex", 20);
    Value b("Bob", 40);
    table1.insert("adqsdasd", a);
    table1.insert("as", b);
    table1.erase("adqsdasd");
    table1.insert("asddaasd", b);
    EXPECT_EQ(table1.size(), 2);
}

TEST(HashTableTest, CheckEmpty) {
    HashTable table1;
    Value a("Alex", 20);
    Value b("Bob", 40);
    table1.insert("adqsdasd", a);
    table1.insert("as", b);
    table1.erase("adqsdasd");
    table1.insert("asddaasd", b);
    EXPECT_FALSE(table1.empty());
}

TEST(HashTableTest, CheckClearEmptyTable) {
    HashTable table;
    table.clear();
    EXPECT_EQ(table.size(), 0);
}

TEST(HashTableTest, CheckClear) {
    HashTable table;
    Value a("Alex", 20);
    Value b("Bob", 40);
    table.insert("asd", a);
    table.insert("ad123", b);
    table.clear();

    EXPECT_EQ(table.size(), 0);
}

TEST(HashTableTest, CheckAt) {
    HashTable table;
    Value a("Alex", 20);
    table.insert("12aa3", a);
    Value c = table.at("12aa3");
    EXPECT_TRUE(c.age == 20 && c.name == "Alex");
}


TEST(HashTableTest, CheckSquareBrackets1) {
    HashTable table;
    Value a("Alex", 20);
    table.insert("nijd", a);
    EXPECT_TRUE((table["nijd"].name == "Alex") && (table["nijd"].age == 20));
}

TEST(HashTableTest, CheckSquareBrackets2) {
    HashTable table;
    std::string key = "nijd";
    EXPECT_TRUE((table[key].name == "") && (table[key].age == 0));
}

TEST(HashTableTest, CheckEqualsNotEmptyTables) {
    HashTable table1;
    HashTable table2;
    Value a("Alex", 20);
    Value b("Bob", 40);

    table1.insert("aadass", a);
    table1.insert("dsaddassf", b);
    table2.insert("aadass", a);
    table2.insert("dsaddassf", b);

    EXPECT_TRUE(table1 == table2);
}


TEST(HashTableTest, CheckEqualsEmptyTables) {
    HashTable table1;
    HashTable table2;
    EXPECT_TRUE(table1 == table2);
}

TEST(HashTableTest, CheckNotEquals) {
    HashTable table1;
    HashTable table2;
    Value a("Alex", 20);
    Value b("Bob", 40);

    table1.insert("aadass", a);
    table1.insert("dsaddassf", b);
    table2.insert("aadas1s", a);
    table2.insert("dsaddassf", b);

    EXPECT_TRUE(table1 != table2);
}

