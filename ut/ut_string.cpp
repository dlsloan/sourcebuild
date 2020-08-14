#include "Base/String.h"
#include "UT/UT.h"

#include <string.h>

using namespace Base;

UT(String_init) {
  String s("");
  UT_ASSERT(strcmp("", s.c_str()) == 0);

  s = String("");
  UT_ASSERT(strcmp("", s.c_str()) == 0);

  s = "abc";
  UT_ASSERT(strcmp("abc", s.c_str()) == 0);

  s = String("123");
  UT_ASSERT(strcmp("123", s.c_str()) == 0);

  String s2 = s;
  UT_ASSERT(strcmp("123", s2.c_str()) == 0);
  UT_ASSERT(strcmp("123", s.c_str()) == 0);
}

UT(String_eq) {
  String s = "123";
  UT_ASSERT(s == "123");
  UT_ASSERT(s != "12");
  UT_ASSERT(s != "23");
  UT_ASSERT(s != "abc");

  String s1 = "123";
  String s2 = "12";
  String s3 = "23";
  String s4 = "abc";
  UT_ASSERT(s == s1);
  UT_ASSERT(s != s2);
  UT_ASSERT(s != s3);
  UT_ASSERT(s != s4);
}

UT(String_substring) {
  String s = "test123";

  UT_ASSERT(s.substring(4) == "123");
  UT_ASSERT(s.substring(0, 4) == "test");
  UT_ASSERT(s.substring(2, 4) == "st12");
}

UT(String_contains) {
  String s = "test123abc";

  UT_ASSERT(s.contains("test"));
  UT_ASSERT(s.contains("123"));
  UT_ASSERT(s.contains("abc"));
  UT_ASSERT(s.contains("test123abc"));
  UT_ASSERT(!s.contains("tester"));
  UT_ASSERT(!s.contains("0123"));
  UT_ASSERT(!s.contains("abcd"));

  String s1 = "test";
  String s2 = "123";
  String s3 = "abc";
  String s4 = "tester";
  String s5 = "0123";
  String s6 = "abcd";
  UT_ASSERT(s.contains(s1));
  UT_ASSERT(s.contains(s2));
  UT_ASSERT(s.contains(s3));
  UT_ASSERT(s.contains(s));
  UT_ASSERT(!s.contains(s4));
  UT_ASSERT(!s.contains(s5));
  UT_ASSERT(!s.contains(s6));
}

UT(String_replace) {
  String s = "test123abc";

  UT_ASSERT(s.replace("test", "") == "123abc");
  UT_ASSERT(s.replace("123", ".") == "test.abc");
  UT_ASSERT(s.replace("abc", "?") == "test123?");
  UT_ASSERT(s.replace("test123abc", "") == "");

  String l1 = "test", r1 = "";
  String l2 = "123", r2 = ".";
  String l3 = "abc", r3 = "?";
  String l4 = "test123abc", r4 = "";
  UT_ASSERT(s.replace(l1, r1) == "123abc");
  UT_ASSERT(s.replace(l2, r2) == "test.abc");
  UT_ASSERT(s.replace(l3, r3) == "test123?");
  UT_ASSERT(s.replace(l4, r4) == "");
}

UT(String_trim) {
  String s1 = "\t\n\r trim me\t\n\r ";
  UT_ASSERT(s1.trim() == "trim me");
  UT_ASSERT(s1.ltrim() == "trim me\t\n\r ");
  UT_ASSERT(s1.rtrim() == "\t\n\r trim me");
  
  s1 = "  ";
  UT_ASSERT(s1.trim() == "");
  UT_ASSERT(s1.ltrim() == "");
  UT_ASSERT(s1.rtrim() == "");

  s1 = "";
  UT_ASSERT(s1.trim() == "");
  UT_ASSERT(s1.ltrim() == "");
  UT_ASSERT(s1.rtrim() == "");

  s1 = "no trim";
  UT_ASSERT(s1.trim() == "no trim");
  UT_ASSERT(s1.ltrim() == "no trim");
  UT_ASSERT(s1.rtrim() == "no trim");
}

UT(String_index_op) {
  String s = "test1234";

  UT_ASSERT(s[0] == 't');
  UT_ASSERT(s[1] == 'e');
  UT_ASSERT(s[6] == '3');
  UT_ASSERT(s[7] == '4');
  UT_ASSERT(s[-1] == '4');
  UT_ASSERT(s[-2] == '3');
  UT_ASSERT(s[-7] == 'e');
  UT_ASSERT(s[-8] == 't');
}

UT(String_indexOf) {
  String s = "test1234";

  UT_ASSERT(s.indexOf("z") == -1);
  UT_ASSERT(s.indexOf("t") == 0);
  UT_ASSERT(s.indexOf("4") == 7);
  UT_ASSERT(s.indexOf("34") == 6);
  UT_ASSERT(s.indexOfR("t") == 3);
  UT_ASSERT(s.indexOfR("34") == 6);
  UT_ASSERT(s.indexOfR("z") == -1);

  String sz = "z";
  String st = "t";
  String s4 = "4";
  String s34 = "34";
  UT_ASSERT(s.indexOf(sz) == -1);
  UT_ASSERT(s.indexOf(st) == 0);
  UT_ASSERT(s.indexOf(s4) == 7);
  UT_ASSERT(s.indexOf(s34) == 6);
  UT_ASSERT(s.indexOfR(st) == 3);
  UT_ASSERT(s.indexOfR(s34) == 6);
  UT_ASSERT(s.indexOfR(sz) == -1);
}

UT(String_split) {
  String s = "test.1234//.hello";

  auto parts = s.split(".");
  UT_ASSERT(parts.count() == 3);
  UT_ASSERT(parts[0] == "test");
  UT_ASSERT(parts[1] == "1234//");
  UT_ASSERT(parts[2] == "hello");

  String sep = "/";
  parts = s.split(sep);
  UT_ASSERT(parts.count() == 3);
  UT_ASSERT(parts[0] == "test.1234");
  UT_ASSERT(parts[1] == "");
  UT_ASSERT(parts[2] == ".hello");
  
  parts = s.split("//");
  UT_ASSERT(parts.count() == 2);
  UT_ASSERT(parts[0] == "test.1234");
  UT_ASSERT(parts[1] == ".hello");
  
  parts = s.split("notThere");
  UT_ASSERT(parts.count() == 1);
  UT_ASSERT(parts[0] == "test.1234//.hello");
}

UT(String_startsWith) {
  String s = "test1234";

  UT_ASSERT(s.startsWith("t"));
  UT_ASSERT(s.startsWith("test"));
  UT_ASSERT(s.startsWith("test1234"));
  UT_ASSERT(!s.startsWith("1234"));

  String s1 = "t";
  String s2 = "test";
  String s3 = "test1234";
  String s4 = "1234";
  UT_ASSERT(s.startsWith(s1));
  UT_ASSERT(s.startsWith(s2));
  UT_ASSERT(s.startsWith(s3));
  UT_ASSERT(!s.startsWith(s4));
}

UT(String_endsWith) {
  String s = "test1234";

  UT_ASSERT(s.endsWith("4"));
  UT_ASSERT(s.endsWith("1234"));
  UT_ASSERT(s.endsWith("test1234"));
  UT_ASSERT(!s.endsWith("test"));

  String s1 = "4";
  String s2 = "1234";
  String s3 = "test1234";
  String s4 = "test";
  UT_ASSERT(s.endsWith(s1));
  UT_ASSERT(s.endsWith(s2));
  UT_ASSERT(s.endsWith(s3));
  UT_ASSERT(!s.endsWith(s4));
}

UT(String_copyTo) {
  char buf[256];
  String s;

  s.copyTo(buf);
  UT_ASSERT(strcmp(buf, s.c_str()) == 0);

  s = "test1234";
  s.copyTo(buf);
  UT_ASSERT(strcmp(buf, s.c_str()) == 0);
}

UT(String_add_op) {
  String s = "test";
  s += "1234";

  UT_ASSERT(s == "test1234");

  s = s + String(".hi");
  UT_ASSERT(s == "test1234.hi");
}