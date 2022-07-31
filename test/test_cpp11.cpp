#include <fast_int/fast_int.hpp>

#include <cstring>

// This code is only here to test that compiling the library with only C++11
// support will work properly.
int main(int argc, char** argv)
{
  std::int32_t i;
  fast_int::from_chars(argv[1], argv[1] + std::strlen(argv[1]), i);
  return 0;
}
