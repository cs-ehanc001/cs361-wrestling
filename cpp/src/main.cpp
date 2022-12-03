#include <iostream>
#include <vector>

#include <supl/utility.hpp>

auto main([[maybe_unused]] const int argc,
          [[maybe_unused]] const char* const* const argv) -> int
{
  std::cout << supl::to_string(std::vector {3, 5, 6, 9}) << '\n';

  return 0;
}
