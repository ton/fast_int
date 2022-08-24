// MIT license; see LICENSE for more details.

#ifndef FAST_INT_HPP
#define FAST_INT_HPP

#include <cstdint>
#include <cstring>
#include <limits>
#include <system_error>
#include <type_traits>

#include <iostream> // Remove!

namespace fast_int {

struct from_chars_result
{
  const char *ptr;
  std::errc ec;
};

namespace detail {

template<typename T>
constexpr std::size_t max_digit_count() noexcept
{
  static_assert(std::is_integral<T>::value, "only integer types are supported");
  static_assert(
      sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8,
      "unexpected integer type");

  return sizeof(T) == 1
             ? 3
             : (sizeof(T) == 2 ? 5 : (sizeof(T) == 4 ? 10 : (std::is_signed<T>::value ? 19 : 20)));
}

template<>
constexpr std::size_t max_digit_count<bool>() noexcept
{
  return 1;
}

template<typename Integer>
bool parse_digit(const std::uint8_t c, Integer &i) noexcept
{
  const auto digit = static_cast<std::uint8_t>(c - '0');
  if (digit > 9) { return false; }
  i = 10 * i + digit;  // might overflow, we will handle the overflow later
  return true;
}

template<typename T>
from_chars_result
from_chars(const char *first, const char *last, T &value, std::true_type /* signed */) noexcept
{
  if (first == last) { return {first, std::errc::invalid_argument}; }

  const bool negative = (*first == '-');
  const char *p = first + negative;
  const char *const start_digits = p;

  std::uint64_t i{0};
  while (p != last && parse_digit(*p, i)) { p++; }

  const std::size_t digit_count = std::size_t(p - start_digits);

  if (digit_count == 0) { return {first, std::errc::invalid_argument}; }
  else if (digit_count > max_digit_count<T>())
  {
    return {start_digits + digit_count, std::errc::result_out_of_range};
  }
  // TODO(ton): the condition below may be more easily written as:
  //
  //   i > (std::numeric_limits<T>::max() + std::uint64_t(negative))
  //
  // but for some reason results in seriously degraded performance when compiled
  // with GCC 9+.
  else if (
      (!negative && i > std::numeric_limits<T>::max()) ||
      (negative && i + 1 > std::numeric_limits<T>::max()))
  {
    return {start_digits + digit_count, std::errc::result_out_of_range};
  }

  value = negative ? (~i + 1) : i;  // convert to a negative number if needed
                                    // (two's complement)

  return {p, std::errc{}};
}

template<typename T>
from_chars_result
from_chars(const char *first, const char *last, T &value, std::false_type /* unsigned */) noexcept
{
  const char *p = first;
  const char *const start_digits = p;

  std::uint64_t i{0};
  while (p != last && parse_digit(*p, i)) { p++; }

  const std::size_t digit_count = std::size_t(p - start_digits);

  if (digit_count == 0) { return {first, std::errc::invalid_argument}; }
  else if (digit_count > max_digit_count<T>() || i > std::numeric_limits<T>::max())
  {
    return {p, std::errc::result_out_of_range};
  }
  else if (digit_count == max_digit_count<std::uint64_t>())
  {
    if (first[0] != std::uint8_t('1') || i <= std::numeric_limits<std::int64_t>::max())
    {
      return {p, std::errc::result_out_of_range};
    }
  }

  value = i;

  return {p, std::errc{}};
}

inline uint32_t swar4(const char *s)
{
  std::uint32_t val;
  std::memcpy(&val, s, 4);

  std::uint32_t t1 = val - 0x30303030;

  std::uint32_t t2a = t1 & 0xffff;
  std::uint32_t t2b = (t1 & 0xffff0000) >> 16;

  std::uint32_t t3a = t2a * 2561;
  std::uint32_t t3b = t2b * 2561;

  std::uint32_t t4 = ((t3a & 0xff00) >> 8) | ((t3b & 0xff00) << 8);

  return (6553601 * t4) >> 16 & 0xffff;
}

inline bool is_digit4_swar(const char *s) noexcept
{
  std::uint32_t val;
  std::memcpy(&val, s, 4);

  return (val & 0xf0f0f0f0u) == 0x30303030u && !((val + 0x06060606u) & 0x40404040u);
}

template<typename T>
from_chars_result
from_chars_swar(const char *first, const char *last, T &value, std::true_type /* signed */) noexcept
{
  if (first == last) { return {first, std::errc::invalid_argument}; }

  const bool negative = (*first == '-');
  const char *p = first + negative;
  const char *const start_digits = p;

  std::uint64_t i{0};
  while (p + 4 <= last && is_digit4_swar(p))
  {
    i = i * 10000 + swar4(p);
    p += 4;
  }

  while (p != last && parse_digit(*p, i)) { p++; }

  const std::size_t digit_count = std::size_t(p - start_digits);
  if (digit_count == 0) { return {first, std::errc::invalid_argument}; }
  else if (digit_count > max_digit_count<T>())
  {
    return {start_digits + digit_count, std::errc::result_out_of_range};
  }
  // TODO(ton): the condition below may be more easily written as:
  //
  //   i > (std::numeric_limits<T>::max() + std::uint64_t(negative))
  //
  // but for some reason results in seriously degraded performance when compiled
  // with GCC 9+.
  else if (
      (!negative && i > std::numeric_limits<T>::max()) ||
      (negative && i + 1 > std::numeric_limits<T>::max()))
  {
    return {start_digits + digit_count, std::errc::result_out_of_range};
  }

  value = negative ? (~i + 1) : i;  // convert to a negative number if needed
                                    // (two's complement)

  return {p, std::errc{}};
}

template<typename T>
from_chars_result from_chars_swar(
    const char *first,
    const char *last,
    T &value,
    std::false_type /* unsigned */) noexcept
{
  const char *p = first;
  const char *const start_digits = first;

  std::uint64_t i{0};
  while (p + 4 <= last && is_digit4_swar(p))
  {
    i = i * 10000 + swar4(p);
    p += 4;
  }

  while (p != last && parse_digit(*p, i)) { p++; }
  const std::size_t digit_count = std::size_t(p - start_digits);

  if (digit_count == 0) { return {p, std::errc::invalid_argument}; }
  else if (digit_count > max_digit_count<T>() || i > std::numeric_limits<T>::max())
  {
    return {p, std::errc::result_out_of_range};
  }
  else if (digit_count == max_digit_count<std::uint64_t>())
  {
    if (first[0] != std::uint8_t('1') || i <= std::numeric_limits<std::int64_t>::max())
    {
      return {p, std::errc::result_out_of_range};
    }
  }

  value = i;

  return {p, std::errc{}};
}

}

template<typename T>
from_chars_result from_chars(const char *first, const char *last, T &value) noexcept
{
  static_assert(std::is_integral<T>::value, "only integer types are supported");
  return detail::from_chars(first, last, value, std::is_signed<T>{});
}

template<typename T>
from_chars_result from_chars_swar(const char *first, const char *last, T &value) noexcept
{
  static_assert(std::is_integral<T>::value, "only integer types are supported");
  return detail::from_chars_swar(first, last, value, std::is_signed<T>{});
}

}

#endif
