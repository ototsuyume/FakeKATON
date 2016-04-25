#include "engine.h"

#define ERROR(msg, ... ) {fprintf(stderr, "ERROR [%d] "msg, __LINE__, ##__VA_ARGS__);exit(0);}
#define LOG(msg, ... ) {fprintf(stderr,"INFO [%d] "msg, __LINE__, ##__VA_ARGS__);}

#define CHECK_STR(a, b) {\
  if(a!=b)\
    LOG("Check error. %s != %s\n", a.c_str(), b.c_str()); \
}

#define CHECK_INT(a, b) {\
  if(a!=b)\
    LOG("Check error. %d != %d\n", a, b); \
}

#define CHECK_FLOAT(a, b) {\
  if((a>b&&a-b>0.000001)||(b>a&&b-a>0.000001)) \
    LOG("Check error. %.03f != %.03f\n", a, b); \
}

void WriteData(shared_ptr<FakeKATON::Engine> db,
  const string &key, const string &address, int64_t phone, double data){
  if (!db->WriteString(key, "address", address))
    ERROR("Write string failed.\n");
  if (!db->WriteInt(key, "phone", phone))
    ERROR("Write int failed.\n");
  if (!db->WriteFloat(key, "data", data))
    ERROR("Write float failed.\n");
}

void ReadData(shared_ptr<FakeKATON::Engine> db,
  const string &key, string &address, int64_t &phone, double &data) {
  if (!db->ReadString(key, "address", address))
    ERROR("Read string failed.\n");
  if (!db->ReadInt(key, "phone", phone))
    ERROR("Read int failed.\n");
  if (!db->ReadFloat(key, "data", data))
    ERROR("Read float failed.\n");
}

int main(){
  unordered_map<string, FakeKATON::RecordType> fields;
  fields["address"] = FakeKATON::RecordType::rtString;
  fields["phone"] = FakeKATON::RecordType::rtInt;
  fields["data"] = FakeKATON::RecordType::rtFloat;
  auto db = FakeKATON::Engine::NewEngine(fields);

  // Test basic operation.
  string key = "Andrew", addr = "101 No.1 Street", eaddr;
  int64_t phone = 2937123, ephone;
  double data = 3.1415, edata;
  WriteData(db, key, addr, phone, data);
  ReadData(db, key, eaddr, ephone, edata);
  
  CHECK_STR(addr, eaddr);
  CHECK_INT(phone, ephone);
  CHECK_FLOAT(data, edata);

  return 0;
}