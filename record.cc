#include "record.h"

namespace FakeKATON{
  

  Record::Record(const shared_ptr<vector<FieldDef>> field, 
    const shared_ptr<unordered_map<string, int>> f2p) :
    fields_(field), field2pos_(f2p), begin_(0), end_(TIMESTAMP_INFINITY){
    val_.assign(field->size(), shared_ptr<FieldBase>(nullptr));
  }

  bool Record::SetFieldValue(const string &field, shared_ptr<FieldBase> val){
    if (field2pos_->find(field) == field2pos_->end() || !val)
      return false;
    int pos = field2pos_->at(field);
    val_[pos] = val;
    return true;
  }

  bool Record::SetPrev(shared_ptr<Record> newprev){
    prev_ = newprev;
    return true;
  }

  bool Record::SetendToTid(Tid tid){
    uint64_t new_ts = MAKE_TX_TIMESTAMP(tid), expect = TIMESTAMP_INFINITY;
    return end_.compare_exchange_strong(expect, new_ts);
  }

  bool Record::SetbeginToTid(Tid tid){
    uint64_t new_ts = MAKE_TX_TIMESTAMP(tid), expect = 0;
    return begin_.compare_exchange_strong(expect, new_ts);
  }

  bool Record::SetbeginToTimestamp(Tid tid, uint64_t ts) {
    uint64_t expect = MAKE_TX_TIMESTAMP(tid);
    return begin_.compare_exchange_strong(expect, ts);
  }

  bool Record::CheckVisibility(Tid tid, uint64_t ts) const {
    uint64_t begin = begin_.load(), end = end_.load();
    if (MARK_WITH_TX(begin)){
      if (GET_TX_ID(begin) == tid || end == TIMESTAMP_INFINITY)
        return true;
       return false;
    }
    else if (MARK_WITH_TX(end)){
      if (GET_TX_ID(end) == tid)
        return true;
      return false;
    }

    return begin <= ts&&end >= ts;
  }

  bool Record::Commit(Tid tid, uint64_t ts){
    uint64_t begin = begin_.load(), end = end_.load();
    if (MARK_WITH_TX(begin))
      return begin_.compare_exchange_strong(begin, ts) && 
        end_.compare_exchange_strong(end, TIMESTAMP_INFINITY);
    else if (MARK_WITH_TX(end))
      return end_.compare_exchange_strong(end, ts);
    return false;
  }

  void Record::Restore() {
    end_.store(TIMESTAMP_INFINITY);
  }

  void Record::SetUnvisible(){
    begin_.store(TIMESTAMP_INVALID);
    end_.store(TIMESTAMP_INVALID);
  }
}