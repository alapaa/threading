#include "thread_safety_mutex.h"

// May need
// sudo apt install libc++-dev
// clang++ -stdlib=libc++ <rest of arguments>
// and possibly static linking of libc++abi.
//#include "clanghack.h"

class Foo {
public:
  void set_val(int val) {
    MutexLocker lg(&mu_);
    val_ = val;
  }
  void set_val_with_race_condition(int val) {
    val_ = val;
  }
private:
  Mutex mu_;
  int val_ GUARDED_BY(mu_);
  //int val_ THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(mu_));
};

int main() {
  Foo foo;
  foo.set_val(5);
  foo.set_val_with_race_condition(5);
}
