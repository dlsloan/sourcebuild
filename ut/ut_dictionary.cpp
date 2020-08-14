#include "Base/String.h"
#include "Base/Dictionary.h"
#include "UT/UT.h"

using namespace Base;

UT(Dictionary_Init) {
  Dictionary<String, String> dict;

  UT_ASSERT(dict.count() == 0);
}

UT(Dictionary_Add) {
  Dictionary<String, String> dict(1);

  dict.add("1", "abc");
  UT_ASSERT(dict.count() == 1);
  UT_ASSERT(dict["1"] == "abc");

  dict.add("2", "hello?");
  UT_ASSERT(dict.count() == 2);
  UT_ASSERT(dict["2"] == "hello?");
}

UT(Dictionary_Copy) {
  Dictionary<String, String> dict1;
  dict1.add("2", "b");
  dict1.add("1", "a");

  Dictionary<String, String> dict2(dict1);
  UT_ASSERT(dict2.count() == 2);
  UT_ASSERT(dict2["1"] == "a");
  UT_ASSERT(dict2["2"] == "b");

  dict2 = dict1;
  UT_ASSERT(dict2.count() == 2);
  UT_ASSERT(dict2["1"] == "a");
  UT_ASSERT(dict2["2"] == "b");
}