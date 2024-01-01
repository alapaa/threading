#include <iostream>
#include <memory>
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
    int result = usleep(ONE_MILLION);
    if (result == -1) {
      std::cerr << "usleep error: " << strerror(errno) << '\n';
      break;
    }
    arg -= ONE_MILLION;
  }
  std::cout << "Leaving tfun()\n";
}

int main()
{
  std::thread t(tfun, TEN_MILLION);
  usleep(FIVE_MILLION);
  auto handle = t.native_handle();
  t.detach();
  //t.join();
  int result = pthread_cancel(handle);
  if (result) {
    std::cerr << "pthread_cancel failed: " << strerror(result) << '\n';
  }
  std::cout << "Leaving main()\n";
  pthread_exit(nullptr);
}
