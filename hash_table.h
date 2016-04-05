#ifndef FakeKATON_HASH_TABLE_H_
#define FakeKATON_HASH_TABLE_H_

#include <atomic>
#include <cstdint>
#include <vector>
#include <memory>

using namespace std;

namespace FakeKATON{
  template <typename KEY, typename VAL>
  class HashTable{
    enum class EntryType{
      EEmpty,
      EDeleted,
      ENormal,
    };

    //template<typename KEY, typename VAL>
    struct HashEntry{
      atomic<EntryType> t_;
      shared_ptr<KEY> key_;
      shared_ptr<VAL> val_;
    };

  public:
    HashTable(size_t);
    virtual ~HashTable();

    bool GetValue(const KEY&, VAL&) const;
    bool SetValue(const KEY&, const VAL&);

  private:
    vector<HashEntry> table_;
    atomic<size_t> count_;
  };

  template<typename KEY, typename VAL>
  HashTable<KEY, VAL>::HashTable(size_t init_size) :count_(0),
    table_(vector<HashEntry>(init_size)){
  }

  template<typename KEY,typename VAL>
  bool HashTable<KEY, VAL>::GetValue(const KEY &key, VAL &val) const{
    
    int hash_val = hash<KEY>()(key);
    size_t size = table_.size(),reprobe=0;
    int index = hash_val % size;
    while (true){
      const HashEntry &entry = table_[index];
      switch (entry.t_.load()){
      case EntryType::EEmpty:
        return false;
      case EntryType::ENormal:
        if (key == *entry.key_){
          val = *entry.val_;
        }
      case EntryType::EDeleted:
        if (++reprobe == size)
          return false;
        index = (size + 1) % size;
        continue;
      }
    }
    // Should not be here.
    return false;
  }

  template<typename KEY,typename VAL>
  bool HashTable<KEY, VAL>::SetValue(const KEY &key,  const VAL &val){
    int hash_val = hash<KEY>()(key);
    size_t size = table_.size() / 2, reprobe = 0;
    int index = hash_val % size;

    auto pval = shared_ptr<VAL>(new VAL(val));

    while (true){
      HashEntry &entry = table_[index];
      switch (entry.t_.load()){
      case EntryType::EEmpty:
      {
        shared_ptr<VAL> expect = nullptr;
        if (atomic_compare_exchange_strong(&entry.val_, &expect, pval)){
          entry.key_= shared_ptr<KEY>(new KEY(key));
          entry.t_.store(EntryType::ENormal);
          return true;
        }
      }
      case EntryType::ENormal:
        if (key == *entry.key_){
          entry.val_ = pval;
          return true;
        }
      case EntryType::EDeleted:
        break;
      }
      if (++reprobe == size)
        return false;
      index = (size + 1) % size;
    }
    return false;
  }
}

#endif