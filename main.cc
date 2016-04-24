#include "engine.h"

#define ERROR(msg, ... ) {fprintf(stderr, "ERROR [%d] "msg, __LINE__, ##__VA_ARGS__);exit(0);}

int main(){
  unordered_map<string, FakeKATON::RecordType> fields;
  fields["address"] = FakeKATON::RecordType::rtString;
  fields["phone"] = FakeKATON::RecordType::rtInt;
  fields["data"] = FakeKATON::RecordType::rtFloat;
  auto db = FakeKATON::Engine::NewEngine(fields);
  string key = "Andrew", field = "address", value = "101 No.1 Street",rval;
  if (!db->WriteString(key, field, value))
    ERROR("Write string failed.\n");
  field = "phone";
  if (!db->WriteInt(key, field, 1234567))
    ERROR("Write int failed.\n");
  field = "data";
  if (!db->WriteFloat(key, field, 1.23456))
    ERROR("Write float failed.\n");

  if (!db->ReadString(key, field, rval))
    ERROR("Read string failed.\n");
  printf("Read address: %s\n",rval.c_str());
  
  field = "address";
  value = "102 No. 2 Street";
  if (!db->WriteString(key, field, value))
    ERROR("Write string failed.\n");
  if (!db->ReadString(key, field, rval))
    ERROR("Read string failed.\n");
  printf("Read: %s\n", rval);

  int64_t iv;
  field = "phone";
  if (!db->ReadInt(key,field,iv))
    ERROR("Read int failed.\n");
  printf("Read: %d\n", iv);

  double fv;
  field = "data";
  if (!db->ReadFloat(key, field, fv))
    ERROR("Read float failed.\n");
  printf("Read: %.3f\n", fv);
  return 0;
}