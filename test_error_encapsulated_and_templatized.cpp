#include <iostream>
#include <string>
#include <variant>

// Inspired by absl::StatusOr. Return a result or an error message.
// See https://abseil.io/docs/cpp/guides/status.

namespace cloud {
namespace utils {
using ErrMsg = std::string;

template <typename T, typename Base = std::variant<T, ErrMsg>>
class StatusOr : private Base {
 public:
  using Base::Base;
  bool ok() const { return std::holds_alternative<T>(*this); }
  bool fail() const { return std::holds_alternative<ErrMsg>(*this); }
  const T& value() const { return std::get<T>(*this); }
  const ErrMsg& error() const { return std::get<ErrMsg>(*this); }
};



// Usage example:
//
}
}

cloud::utils::StatusOr<int> do_something(int arg) {
  if (arg == 42) return arg*2;
  else return cloud::utils::ErrMsg{"Wrong arg! Cannot compute result."};
}

int main() {
  auto result = do_something(42);
  if (result.fail()) std::cout << result.error() << '\n';
  else std::cout << "Success! Result is " << result.value() << '\n';
  result = do_something(43);
  if (result.fail()) std::cout << result.error() << '\n';
  else std::cout << "Success! Result is" << result.value() << '\n';
}
