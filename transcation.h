#ifndef FAKEKATON_TRANSCATION_H_
#define FAKEKATON_TRANSCATION_H_

#include "common.h"
#include "table.h"

#include <cstdint>
#include <string>
#include <memory>

using namespace std;

namespace FakeKATON{
  
  class Transcation{
  public:
    static shared_ptr<Transcation> BeginTranscation(Tid tid, uint64_t begin,
      shared_ptr<Table> table);
    virtual bool WriteInt(const string &key, const string &field, int64_t val) = 0;
    virtual bool WriteFloat(const string &key, const string &field, double val) = 0;
    virtual bool WriteString(const string &key, const string &field, const string &val) = 0;

    virtual bool ReadInt(const string &key, const string &field, int64_t &val) = 0;
    virtual bool ReadFloat(const string &key, const string &field, double &val) = 0;
    virtual bool ReadString(const string &key, const string &field, string &val) = 0;
    virtual bool Commit(uint64_t end) = 0;
  };

}
#endif