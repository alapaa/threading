#include <iostream>
#include <string>
#include <variant>

typedef std::string ErrMsg;

std::variant<int, ErrMsg> do_something(int arg) {
  if (arg == 42) return arg*2;
  else return ErrMsg{"Wrong arg! Cannot compute result."};
}

int main() {
  auto result = do_something(42);
  if (std::holds_alternative<ErrMsg>(result)) std::cout << std::get<ErrMsg>(result) << '\n';
  else std::cout << "Success! Result is " << std::get<int>(result) << '\n';

  result = do_something(43);
  if (std::holds_alternative<ErrMsg>(result)) std::cout << std::get<ErrMsg>(result) << '\n';
  else std::cout << "Success! Result is" << std::get<int>(result) << '\n';
}
