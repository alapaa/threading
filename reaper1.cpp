#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <cstring>

#include <unistd.h>

constexpr int ONE_MILLION = 1000000;
constexpr int FIVE_MILLION = 5000000;
constexpr int TEN_MILLION = 10000000;

struct Data {
  int data[100];
};

class Foo {
public:
  Foo() { p_ = std::make_unique<Data>(); }
  ~Foo() { std::cout << "Foo destructor running.\n"; }
private:
  std::unique_ptr<Data> p_;
};

void tfun(int arg) {
  Foo foo;
  std::cout << "Executing tfun(), will sleep " << arg << " microseconds\n";
  while (arg > 0) {
    std::cout << "usleep()...\n";
    int result = usleep(ONE_MILLION); // usleep() is a Posix
                                      // cancellation point.
    if (result == -1) {
      // We don't get an EINTR or other error from pthread_cancel.
      // TODO: Set cancellation mode on pthread to deferred? According
      // to pthread_setcanceltype documentation, the default mode is
      // already deferred cancellation, i.e. cancellation only at
      // cancellation points.
      std::cerr << "usleep error: " << strerror(errno) << '\n';
      break;
    }
    arg -= ONE_MILLION;
  }
  std::cout << "Leaving tfun()\n";
  // Here is an implicit pthread_exit() according to pthreads documentation.
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: <progname> <mode>\n";
    exit(1);
  }
  std::string mode = std::string(argv[1]);
  
  std::thread t(tfun, TEN_MILLION);
  usleep(FIVE_MILLION);
  auto handle = t.native_handle();
  if (mode == "pthread_exit") {
    t.detach();
  }
  
  int result = pthread_cancel(handle);
  if (result) {
    std::cerr << "pthread_cancel failed: " << strerror(result) << '\n';
  }
  
  if (mode == "std::thread::join") {
    t.join();
  } else if (mode == "pthread_exit") {
    std::cout << "main() thread calling pthread_exit()\n";
    pthread_exit(nullptr); // This allows Foo destructor in tfun() to run
                         // after pthread_cancel (at least in this
                         // experiment).
    std::cout << "main() after return of pthread_exit\n"; // Does not print.
  } else if (mode == "nothing") {
    // Just leave main without any action. Foo destructor will not
    // run, and std::thread destructor will throw, since the thread is
    // neither joined or detached.
  } else {
    std::cerr << "Unknown mode, exiting...\n";
    exit(1);
  }    
}
