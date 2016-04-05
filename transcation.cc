#include "common.h"
#include "table.h"
#include "transcation.h"

#include <cstdint>
#include <string>
#include <unordered_set>

using namespace std;

namespace FakeKATON{

  class TranscationImpl :public Transcation{
  public:
    TranscationImpl(Tid tid, uint64_t timestamp, shared_ptr<Table> table);
    ~TranscationImpl();

    virtual bool WriteInt(const string &key, const string &field, int64_t val);
    virtual bool WriteFloat(const string &key, const string &field, double val);
    virtual bool WriteString(const string &key, const string &field, const string &val);

    virtual bool ReadInt(const string &key, const string &field, int64_t &val);
    virtual bool ReadFloat(const string &key, const string &field, double &val);
    virtual bool ReadString(const string &key, const string &field, string &val);
    virtual bool Commit(uint64_t end);

  private:
    bool GetNewRecord(const string &key, shared_ptr<Record> &rd);
    bool ReadRecord(const string &key, shared_ptr<Record> &rd);

  private:
    Tid tid_;
    uint64_t begin_;
    shared_ptr<Table> table_;
    unordered_map<string, shared_ptr<Record>> read_set_, write_set_;
  };

  shared_ptr<Transcation> Transcation::BeginTranscation(Tid tid,
    uint64_t begin, shared_ptr<Table> table){
    return shared_ptr<TranscationImpl>(new TranscationImpl(tid, begin, table));
  }

  TranscationImpl::TranscationImpl(Tid tid, uint64_t begin, shared_ptr<Table> table):
    tid_(tid),table_(table),begin_(begin){

  }

  TranscationImpl::~TranscationImpl(){

  }

  bool TranscationImpl::GetNewRecord(const string &key, shared_ptr<Record> &rd){
    if (write_set_.find(key) == write_set_.end()) {
      rd = table_->NewRecord(key, tid_);
      if (!rd.get())
        return false;
      write_set_[key] = rd;
    }
    else
      rd = write_set_[key];
    return true;
  }

  bool TranscationImpl::ReadRecord(const string &key, shared_ptr<Record> &rd){
    if (write_set_.find(key) != write_set_.end())
      rd = write_set_[key];
    else if (read_set_.find(key) != read_set_.end())
      rd = read_set_[key];
    else{
      rd = table_->ReadRecord(key, tid_, begin_);
      if (!rd.get())
        return false;
      read_set_[key] = rd;
    }
    return true;
  }

  bool TranscationImpl::WriteInt(const string &key, const string &field, int64_t val){
    shared_ptr<Record> rd;
    if (!table_->IsFieldValid(field, RecordType::rtInt)||!GetNewRecord(key, rd))
      return false;
    
    return rd->SetFieldValue(field, shared_ptr<FieldBase>(new FieldInt(val)));
  }

  bool TranscationImpl::WriteFloat(const string &key, const string &field, double val) {
    shared_ptr<Record> rd;
    if (!table_->IsFieldValid(field, RecordType::rtFloat)||!GetNewRecord(key, rd))
      return false;

    return rd->SetFieldValue(field, shared_ptr<FieldBase>(new FieldFloat(val)));
  }

  bool TranscationImpl::WriteString(const string &key, const string &field, const string &val) {
    shared_ptr<Record> rd;
    if (!table_->IsFieldValid(field, RecordType::rtString)||!GetNewRecord(key, rd))
      return false;

    return rd->SetFieldValue(field, shared_ptr<FieldBase>(new FieldString(val)));
  }

  bool TranscationImpl::ReadInt(const string &key, const string &field, int64_t &val){
    shared_ptr<Record> rd;
    if (!table_->IsFieldValid(field, RecordType::rtInt) || !ReadRecord(key, rd))
      return false;
    shared_ptr<FieldBase> pval = rd->GetFieldValue(field);
    const shared_ptr<FieldInt> pint_val = dynamic_pointer_cast<FieldInt, FieldBase>(pval);
    val = pint_val->GetValue();
    return true;
  }

  bool TranscationImpl::ReadFloat(const string &key, const string &field, double &val){
    shared_ptr<Record> rd;
    if (!table_->IsFieldValid(field, RecordType::rtFloat) || !ReadRecord(key, rd))
      return false;
    shared_ptr<FieldBase> pval = rd->GetFieldValue(field);
    const shared_ptr<FieldFloat> pint_val = dynamic_pointer_cast<FieldFloat, FieldBase>(pval);
    val = pint_val->GetValue();
    return true;
  }

  bool TranscationImpl::ReadString(const string &key, const string &field, string &val){
    shared_ptr<Record> rd;
    if (!table_->IsFieldValid(field, RecordType::rtString) || !ReadRecord(key, rd))
      return false;
    shared_ptr<FieldBase> pval = rd->GetFieldValue(field);
    const shared_ptr<FieldString> pint_val = dynamic_pointer_cast<FieldString, FieldBase>(pval);
    val = pint_val->GetValue();
    return true;
  }

  bool TranscationImpl::Commit(uint64_t end) {
    for (const auto &readrd : read_set_){
      if (!readrd.second->CheckVisibility(tid_, begin_))
        return false;   // Never reach here.
    }

    for (const auto &writerd : write_set_){
      if (!table_->Commit(writerd.first, tid_, end))
        return false;
    }
    return true;
  }
}