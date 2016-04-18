#ifndef FakeKATON_ENGINE_H_
#define FakeKATON_ENGINE_H_

#include"transcation.h"

namespace FakeKATON{

  class Engine{
  public:
    static shared_ptr<Engine> NewEngine(const unordered_map<string, RecordType> &fields);

    virtual shared_ptr<Transcation> BeginTranscation() = 0;
    virtual bool WriteInt(const string &key, const string &field, int64_t val) = 0;
    virtual bool WriteFloat(const string &key, const string &field, double val) = 0;
    virtual bool WriteString(const string &key, const string &field, const string &filed) = 0;

    virtual bool ReadInt(const string &key, const string &field, int64_t &val) = 0;
    virtual bool ReadFloat(const string &key, const string &field, double &val) = 0;
    virtual bool ReadString(const string &key, const string &field, string &val) = 0;
  };

}

#endif