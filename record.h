#ifndef FAKEKATON_RECORD_H_
#define FAKEKATON_RECORD_H_

#include "common.h"

#include <atomic>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;

namespace FakeKATON{
  class Table;

  class FieldBase{
  public:
    virtual RecordType GetType() = 0;
    virtual shared_ptr<FieldBase> CopyValue() = 0;
  };

  class FieldString :public FieldBase{
  public:
    FieldString(const string &val) :val_(val){};

    virtual RecordType GetType(){
      return RecordType::rtString;
    }

    virtual shared_ptr<FieldBase> CopyValue() {
      return shared_ptr<FieldBase>(new FieldString(val_));
    }

    const string& GetValue(){
      return val_;
    }

    void SetValue(const string &val){
      val_ = val;
    }

  private:
    string val_;
  };

  class FieldInt :public FieldBase{
  public:
    FieldInt(int64_t val) :val_(val){};

    virtual RecordType GetType(){
      return RecordType::rtInt;
    }

    virtual shared_ptr<FieldBase> CopyValue() {
      return shared_ptr<FieldBase>(new FieldInt(val_));
    }

    int64_t GetValue() const{
      return val_;
    }

    void SetValue(int64_t val){
      val_ = val;
    }

  private:
    int64_t val_;
  };

  class FieldFloat :public FieldBase{
  public:
    FieldFloat(double val) :val_(val){};

    virtual RecordType GetType(){
      return RecordType::rtFloat;
    }

    virtual shared_ptr<FieldBase> CopyValue() {
      return shared_ptr<FieldBase>(new FieldFloat(val_));
    }

    double GetValue(){
      return val_;
    }

    void SetValue(double val){
      val_ = val;
    }

  private:
    double val_;
  };

  class Record{
  public:
    Record(const shared_ptr<vector<FieldDef>> field, 
      const shared_ptr<unordered_map<string, int>> f2p);

    bool SetFieldValue(const string &field, shared_ptr<FieldBase> val);

    void SetUnvisible();

    bool CheckVisibility(Tid tid, uint64_t ts) const;
    const shared_ptr<vector<FieldDef>> GetFields() const;
    const shared_ptr<FieldBase> GetFieldValue(const string& field) const;

  private:
    friend class Table;

    // Set the end timestamp to infinity.
    void Restore();

    bool SetendToTid(Tid tid);
    bool SetbeginToTid(Tid tid);
    bool SetbeginToTimestamp(Tid tid, uint64_t ts);

    bool SetPrev(shared_ptr<Record> newprev);
    shared_ptr<Record> GetPrev();

    bool Commit(Tid tid, uint64_t);

  private:
    atomic<uint64_t> begin_, end_;
    shared_ptr<Record> prev_;
    vector<shared_ptr<FieldBase>> val_;

    const shared_ptr<vector<FieldDef>> fields_;
    const shared_ptr<unordered_map<string, int>> field2pos_;
  };
  
}

#endif