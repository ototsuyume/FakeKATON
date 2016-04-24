#ifndef FAKEKATON_COMMON_H_
#define FAKEKATON_COMMON_H_

#include <cstdint>
#include <string>

#define TIMESTAMP_INVALID 0x7ffffffffffffffULL
#define TIMESTAMP_INFINITY  0x0ffffffffffffffULL
#define TIMESTAMP_TX_MASK 0x8000000000000000ULL
#define MAKE_TX_TIMESTAMP(id) (TIMESTAMP_TX_MASK | id)
#define MARK_WITH_TX(ts) (ts >> 63)
#define GET_TX_ID(ts) (ts & 0x7fffffffffffffffULL)

namespace FakeKATON{
  using namespace std;

  enum class RecordType{
    rtInt,
    rtFloat,
    rtString,
  };

  typedef int64_t Tid;


  struct FieldDef{
    string fname_;
    RecordType rt_;
  };
}

#endif