#include "engine.h"

int main(){
  unordered_map<string, FakeKATON::RecordType> fields;
  fields["address"] = FakeKATON::RecordType::rtString;
  fields["phone"] = FakeKATON::RecordType::rtInt;
  fields["data"] = FakeKATON::RecordType::rtFloat;
  auto db = FakeKATON::Engine::NewEngine(fields);
  if (db->WriteString("Andrew", "address", "101 No.1 Street"))
    fprintf(stderr, "Write string failed.\n");
  return 0;
}