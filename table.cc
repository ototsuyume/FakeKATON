#include "table.h"

namespace FakeKATON{
  Table::Table(size_t size, const unordered_map<string, RecordType> &fields) :index_(size)
    ,field2pos_(new unordered_map<string,int>()) {
    fields_ = shared_ptr<vector<FieldDef>>(new vector<FieldDef>());
    for (const auto &item : fields)
      fields_->emplace_back(FieldDef{ item.first, item.second });
    for (size_t i = 0; i < fields_->size(); i++)
      (*field2pos_)[fields_->at(i).fname_] = i;
  }

  Table::~Table() {
    // We should delete the content in index, but our lightweight hash_table
    // doesn't support iteration yet.
  }

  shared_ptr<Record> Table::NewRecord(const string &key, Tid tid) {
    shared_ptr<IndexEntry> entry;
    shared_ptr<Record> head = nullptr, ret(new Record(fields_, field2pos_));
    if (!index_.GetValue(key, entry)){
      entry = shared_ptr<IndexEntry>(new IndexEntry());
      index_.SetValue(key, entry);
    }
    head = entry->data_;
    if (head.get() && !head->SetendToTid(tid))
        return nullptr;
    ret->SetPrev(head);
    ret->SetbeginToTid(tid);
    if (head.get()){
      for (auto iter = fields_->begin();
        iter != fields_->end(); iter++){
        auto val = head->GetFieldValue(iter->fname_);
        if (val.get()){
          auto cval = val->CopyValue();
          ret->SetFieldValue(iter->fname_, cval);
        }
      }
    }
    
    if (atomic_compare_exchange_strong(&entry->data_, &head, ret))
      return ret;
    else
      return nullptr;
  }

  const shared_ptr<Record> Table::ReadRecord(const string &key, Tid tid, uint64_t ts) const{
    shared_ptr<IndexEntry> entry;
    shared_ptr<Record> cur = nullptr;
    if (!index_.GetValue(key, entry))
      return nullptr;
    cur = entry->data_;
    while (cur.get()){
      if (cur->CheckVisibility(tid, ts))
        return cur;
      cur = cur->GetPrev();
    }
    return nullptr;
  }

  bool Table::Commit(const string &key, Tid tid, uint64_t ts) {
    shared_ptr<IndexEntry> entry;
    shared_ptr<Record> head = nullptr;
    if (!index_.GetValue(key, entry))
      return false;
    head = entry->data_;
    if (!head || !head->Commit(tid, ts)){
      entry->data_ = head->GetPrev();
      return false;
    }
    auto prev = head->GetPrev();
    /*
    // We haven't implemented the commit dependency graph, we don't set a record 
    // to unvisible.

    if (!prev.get())
      prev->SetUnvisible();
    */
    head->SetPrev(nullptr);
    return true;
  }

  bool Table::IsFieldValid(const string &field, RecordType rt) const {
    return field2pos_->find(field) != field2pos_->end() && 
      fields_->at(field2pos_->at(field)).rt_ == rt;
  }
}