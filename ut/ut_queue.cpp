#include "Base/String.h"
#include "Base/Queue.h"
#include "UT/UT.h"

using namespace Base;

UT(Queue_Init) {
  Queue<String> sq(1);
  UT_ASSERT(sq.size() == 1);

  Queue<int> iq(2);
  UT_ASSERT(iq.size() == 2);
}

UT(Queue_enqueue) {
  Queue<String> sq(1);
  sq.enqueue("test");
  UT_ASSERT(sq.count() == 1);
  UT_ASSERT(sq[0] == "test");

  sq.enqueue("test1");
  UT_ASSERT(sq.count() == 2);
  UT_ASSERT(sq[1] == "test1");
}

UT(Queue_dequeue) {
  Queue<String> sq(1);
  sq.enqueue("test");
  sq.enqueue("test1");
  UT_ASSERT(sq.dequeue() == "test");
  UT_ASSERT(sq.dequeue() == "test1");
}