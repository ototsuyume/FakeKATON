#include "gtest/gtest.h"
#include"hash_table.h"

class HashTableTest :public testing::Test{
public:
  HashTableTest() :table_(1024){};

protected:
  virtual void SetUp(){
    table_.SetValue("key1", "value1");
    table_.SetValue("key2", "value2");
  }

  FakeKATON::HashTable<string, string> table_;
};

TEST_F(HashTableTest, GetValue){
  string expected;
  EXPECT_TRUE(table_.GetValue("key1", expected));
  EXPECT_EQ("value1", expected);

  EXPECT_TRUE(table_.GetValue("key2", expected));
  EXPECT_EQ("value2", expected);

  EXPECT_FALSE(table_.GetValue("key3", expected));
}

TEST_F(HashTableTest, SetValue){
  string new_val = "New value.", expected;
  EXPECT_TRUE(table_.SetValue("key1", new_val));

  EXPECT_TRUE(table_.GetValue("key1", expected));
  EXPECT_EQ(new_val, expected);

  EXPECT_TRUE(table_.GetValue("key2", expected));
  EXPECT_EQ("value2", expected);

  EXPECT_TRUE(table_.GetValue("key2", expected));
  EXPECT_EQ(new_val, expected);
}