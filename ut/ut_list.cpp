#include "Base/List.h"
#include "Base/String.h"
#include "UT/UT.h"

using namespace Base;

UT(List_Init) {
  List<String> lst(1);

  UT_ASSERT(lst.count() == 0);
  UT_ASSERT(lst.size() == 1);

  List<int> ilist(1);
  UT_ASSERT(ilist.count() == 0);
  UT_ASSERT(lst.size() == 1);
}

UT(List_Add) {
  List<String> slist(1);
  slist.add("1");
  UT_ASSERT(slist.count() == 1);
  UT_ASSERT(slist.size() == 1);
  UT_ASSERT(slist[0] == "1");

  slist += "2";
  UT_ASSERT(slist.count() == 2);
  UT_ASSERT(slist.size() >= 2);
  UT_ASSERT(slist[1] == "2");

  List<String> slist2;
  slist2 += "3";
  slist2 += "4";
  slist += slist2;
  UT_ASSERT(slist.count() == 4);
  UT_ASSERT(slist.size() >= 4);
  UT_ASSERT(slist[2] == "3");
  UT_ASSERT(slist[3] == "4");

  List<int> ilist(1);
  ilist.add(1);
  UT_ASSERT(ilist.count() == 1);
  UT_ASSERT(ilist.size() == 1);
  UT_ASSERT(ilist[0] == 1);

  ilist += 2;
  UT_ASSERT(ilist.count() == 2);
  UT_ASSERT(ilist.size() >= 2);
  UT_ASSERT(ilist[1] == 2);
}

UT(List_set_op) {
  List<String> slist;
  slist += "abc";
  slist += "123";
  slist += "test";

  List<String> list = slist;
  UT_ASSERT(list.count() == 3);
  UT_ASSERT(list[0] == "abc");
  UT_ASSERT(list[1] == "123");
  UT_ASSERT(list[2] == "test");

  List<String> slist2;
  slist2 += "abcd";
  slist2 += "1234";
  slist2 += "tester";

  list = slist2;
  UT_ASSERT(list.count() == 3);
  UT_ASSERT(list[0] == "abcd");
  UT_ASSERT(list[1] == "1234");
  UT_ASSERT(list[2] == "tester");
}

UT(List_iter) {
  List<String> slist;
  slist += "test";
  slist += "123";
  slist += "abc";

  auto iter = slist.iter();
  UT_ASSERT(iter.valid());
  UT_ASSERT(iter.value() == "test");
  iter.next();
  UT_ASSERT(iter.valid());
  UT_ASSERT(iter.value() == "123");
  iter.next();
  UT_ASSERT(iter.valid());
  UT_ASSERT(iter.value() == "abc");
  iter.next();
  UT_ASSERT(!iter.valid());
}