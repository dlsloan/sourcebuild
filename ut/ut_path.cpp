#include "FileSystem/Path.h"
#include "UT/UT.h"

using namespace FileSystem;

UT(Path_init) {
  Path p("test/123");

  UT_ASSERT(p.toString() == "test/123");

  
  Path p2;
  UT_ASSERT(p2.toString() == "");
}

UT(Path_add) {
  Path p("test/123");
  p += ".test";

  UT_ASSERT(p.length() == 2);
  UT_ASSERT(p[0] == "test");
  UT_ASSERT(p[1] == "123.test");

  Path p1;
  p1 += "file";
  UT_ASSERT(p1.length() == 1);
  UT_ASSERT(p1[0] == "file");
}

UT(Path_div) {
  Path p;
  p = p / "test";
  UT_ASSERT(p.toString() == "test");

  p = Path("/");
  p = p / "test2";
  UT_ASSERT(p.toString() == "/test2");
}

UT(Path_trimExt) {
  Path p("a/test.123");
  UT_ASSERT(p.trimExt().toString() == "a/test");
  
  p = Path("a/test");
  UT_ASSERT(p.trimExt().toString() == "a/test");

  p = Path("a/test.123.2");
  UT_ASSERT(p.trimExt().toString() == "a/test.123");
}

UT(Path_stem) {
  Path p("a/test.123");
  UT_ASSERT(p.stem() == "test");
}