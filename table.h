#ifndef FAKEKATON_TABLE_H
#define FAKEKATON_TABLE_H

#include "common.h"
#include "hash_table.h"
#include "record.h"
#include "transcation.h"

#include <atomic>
#include <string>
#include <unordered_map>
#include <memory>

using namespace std;

namespace FakeKATON{
 
  struct IndexEntry{
    shared_ptr<Record> data_;
    shared_ptr<IndexEntry> next_;
  };

  class Table{
  public:
    Table(size_t size,  shared_ptr<vector<FieldDef>> fields);
    ~Table();
    
    shared_ptr<Record> NewRecord(const string &key, Tid tid);
    const shared_ptr<Record> ReadRecord(const string &key, Tid tid, uint64_t begin) const;
    bool Commit(const string &key, Tid tid, uint64_t ts);

    bool IsFieldValid(const string &field, RecordType rt) const;

  private:
    HashTable<string, shared_ptr<IndexEntry>> index_;
    shared_ptr<vector<FieldDef>> fields_;
    shared_ptr<unordered_map<string, int>> field2pos_;
  };
}

#endif