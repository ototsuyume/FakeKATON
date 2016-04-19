#ifndef _WIN32
#include <sys/time.h>
#else
#include <ctime>
#endif


#include "engine.h"
#include "table.h"
#include "transcation.h"

namespace FakeKATON{
  class EngineImpl :public Engine{
  public:
    EngineImpl(const unordered_map<string, RecordType> &fields);
    ~EngineImpl();

    virtual shared_ptr<Transcation> BeginTranscation();

    virtual bool WriteInt(const string &key, const string &field, int64_t val);
    virtual bool WriteFloat(const string &key, const string &field, double val);
    virtual bool WriteString(const string &key, const string &field, const string &val);

    virtual bool ReadInt(const string &key, const string &field, int64_t &val);
    virtual bool ReadFloat(const string &key, const string &field, double &val);
    virtual bool ReadString(const string &key, const string &field, string &val);

  private:
    uint64_t GetCurrentTimestamp();

  private:
    atomic<Tid> global_tid_;
    shared_ptr<Table> table_;
  };

  shared_ptr<Engine> Engine::NewEngine(const unordered_map<string, RecordType> &fields){
    return shared_ptr<Engine>(new EngineImpl(fields));
  }

  EngineImpl::EngineImpl(const unordered_map<string, RecordType> &fields){  
    table_ = shared_ptr<Table>(new Table(10240, fields));
  }

  EngineImpl::~EngineImpl(){

  }

  uint64_t EngineImpl::GetCurrentTimestamp(){
    uint64_t timestamp;
#ifndef _WIN32
    timeval tv;
    gettimeofday(&tv,NULL);
    timestamp = 1000000 * tv.tv_sec + tv.tv_usec;

#else
    timestamp = time(nullptr);
#endif
    return timestamp;
  }

  shared_ptr<Transcation> EngineImpl::BeginTranscation(){
    return Transcation::BeginTranscation(global_tid_.fetch_add(1),
      GetCurrentTimestamp(), table_);
  }

  bool EngineImpl::WriteInt(const string &key, const string &field, int64_t val){
    auto tx = BeginTranscation();
    tx->WriteInt(key, field, val);
    return tx->Commit(GetCurrentTimestamp());
  }

  bool EngineImpl::WriteFloat(const string &key, const string &field, double val){
    auto tx = BeginTranscation();
    tx->WriteFloat(key, field, val);
    return tx->Commit(GetCurrentTimestamp());
  }

  bool EngineImpl::WriteString(const string &key, const string &field, const string &val){
    auto tx = BeginTranscation();
    tx->WriteString(key, field, val);
    return tx->Commit(GetCurrentTimestamp());
  }

  bool EngineImpl::ReadInt(const string &key, const string &field, int64_t &val){
    auto tx = BeginTranscation();
    tx->ReadInt(key, field, val);
    return tx->Commit(GetCurrentTimestamp());
  }

  bool EngineImpl::ReadFloat(const string &key, const string &field, double &val){
    auto tx = BeginTranscation();
    tx->ReadFloat(key, field, val);
    return tx->Commit(GetCurrentTimestamp());
  }

  bool EngineImpl::ReadString(const string &key, const string &field, string &val){
    auto tx = BeginTranscation();
    tx->ReadString(key, field, val);
    return tx->Commit(GetCurrentTimestamp());
  }
}