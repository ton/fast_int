#include <fast_int/fast_int.hpp>

#include <catch2/catch_template_test_macros.hpp>

#include <cstdint>

#define FAST_INT_INTEGER_TYPES bool, char, signed char, unsigned char, wchar_t, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long

TEMPLATE_TEST_CASE("Negative number within range", "", FAST_INT_INTEGER_TYPES)
{
  auto input = std::numeric_limits<TestType>::min() / 2;
  std::string s{std::to_string(input)};
  TestType output{};
  fast_int::from_chars_result result = fast_int::from_chars(s.data(), s.data() + s.size(), output);
  REQUIRE(result.ptr == s.data() + s.size());
  REQUIRE(result.ec == std::errc{});
  REQUIRE(input == output);
}

TEMPLATE_TEST_CASE("Positive number within range", "", FAST_INT_INTEGER_TYPES)
{
  auto input = std::numeric_limits<TestType>::max() / 2;
  std::string s{std::to_string(input)};
  TestType output{};
  fast_int::from_chars_result result = fast_int::from_chars(s.data(), s.data() + s.size(), output);
  REQUIRE(result.ptr == s.data() + s.size());
  REQUIRE(result.ec == std::errc{});
  REQUIRE(input == output);
}

TEMPLATE_TEST_CASE("Minimum number", "", FAST_INT_INTEGER_TYPES)
{
  auto input = std::numeric_limits<TestType>::min();
  const std::string s{std::to_string(input)};
  TestType output{};
  fast_int::from_chars_result result = fast_int::from_chars(s.data(), s.data() + s.size(), output);
  REQUIRE(result.ptr == s.data() + s.size());
  REQUIRE(result.ec == std::errc{});
  REQUIRE(input == output);
}

TEMPLATE_TEST_CASE("Maximum number", "", FAST_INT_INTEGER_TYPES)
{
  auto input = std::numeric_limits<TestType>::max();
  const std::string s{std::to_string(input)};
  TestType output{};
  fast_int::from_chars_result result = fast_int::from_chars(s.data(), s.data() + s.size(), output);
  REQUIRE(result.ptr == s.data() + s.size());
  REQUIRE(result.ec == std::errc{});
  REQUIRE(input == output);
}

TEMPLATE_TEST_CASE("Overflow", "", FAST_INT_INTEGER_TYPES)
{
  auto input = std::numeric_limits<TestType>::max();
  const std::string s{std::to_string(input) + "0"};
  TestType output{};
  fast_int::from_chars_result result = fast_int::from_chars(s.data(), s.data() + s.size(), output);
  REQUIRE(result.ptr == s.data() + s.size());
  REQUIRE(result.ec == std::errc::result_out_of_range);
  REQUIRE(output == 0);
}

TEMPLATE_TEST_CASE("Underflow", "", FAST_INT_INTEGER_TYPES)
{
  auto input = std::numeric_limits<TestType>::min();
  const std::string s{std::is_signed<TestType>::value ? std::to_string(input) + "1" : "-1"};
  TestType output{};
  fast_int::from_chars_result result = fast_int::from_chars(s.data(), s.data() + s.size(), output);

  if (std::is_signed<TestType>::value)
  {
    REQUIRE(result.ptr == s.data() + s.size());
    REQUIRE(result.ec == std::errc::result_out_of_range);
    REQUIRE(output == 0);
  }
  else
  {
    REQUIRE(result.ptr == s.data());
    REQUIRE(result.ec == std::errc::invalid_argument);
    REQUIRE(output == 0);
  }
}

// Separate test case for overflow handling of a 64-bit integer type; this hits
// a slightly different code path than the other overflow test.
TEST_CASE("OverflowUInt64")
{
  const std::string s{"18446744073709551617"};
  std::uint64_t output{};
  fast_int::from_chars_result result = fast_int::from_chars(s.data(), s.data() + s.size(), output);
  REQUIRE(result.ptr == s.data() + s.size());
  REQUIRE(result.ec == std::errc::result_out_of_range);
  REQUIRE(output == 0);
}
