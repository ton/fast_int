#include <benchmark/benchmark.h>
#include <fast_int/fast_int.hpp>

#include <charconv>
#include <cstdint>
#include <cstring>
#include <map>

namespace {

template<typename T>
struct Input
{
  std::vector<std::string> numbers;
  T expected_sum{};
};

template<typename T>
Input<T> generate_input(int range)
{
  static std::map<int, Input<T>> inputs;

  auto [it, inserted] = inputs.emplace(range, Input<T>{});

  if (inserted)
  {
    std::vector<std::string> &numbers{it->second.numbers};
    T &sum{it->second.expected_sum};

    constexpr int N{1000};
    for (int i{0}; i < N; ++i)
    {
      const bool negate = std::is_signed<T>::value && std::rand() % 2 == 0;
      T number = (negate ? -1 : 1) * std::rand() % range;
      sum += number;
      numbers.push_back(std::to_string(number));
    }
  }

  return it->second;
}

}

template<typename T>
static void BM_fast_int(benchmark::State &state)
{
  Input<T> input = generate_input<T>(state.range(0));

  for (auto _ : state)
  {
    T sum{0};
    for (const std::string &number : input.numbers)
    {
      T i{};
      fast_int::from_chars(number.data(), number.data() + number.size(), i);
      sum += i;
    }

    if (input.expected_sum != sum) { state.SkipWithError("issue in number conversion"); }
  }
}

template<typename T>
static void BM_fast_int_swar(benchmark::State &state)
{
  Input<T> input = generate_input<T>(state.range(0));

  for (auto _ : state)
  {
    T sum{0};
    for (const std::string &number : input.numbers)
    {
      T i{};
      fast_int::from_chars_swar(number.data(), number.data() + number.size(), i);
      sum += i;
    }

    if (input.expected_sum != sum) { state.SkipWithError("issue in number conversion"); }
  }
}

template<typename T>
static void BM_from_chars(benchmark::State &state)
{
  Input<T> input = generate_input<T>(state.range(0));

  for (auto _ : state)
  {
    T sum{0};
    for (const std::string &number : input.numbers)
    {
      T i{};
      std::from_chars(number.data(), number.data() + number.size(), i);
      sum += i;
    }

    if (input.expected_sum != sum) { state.SkipWithError("issue in number conversion "); }
  }
}

BENCHMARK(BM_fast_int<std::int64_t>)->RangeMultiplier(10)->Range(std::int64_t{10}, 1000000);
BENCHMARK(BM_fast_int_swar<std::int64_t>)->RangeMultiplier(10)->Range(std::int64_t{10}, 1000000);
BENCHMARK(BM_from_chars<std::int64_t>)->RangeMultiplier(10)->Range(std::int64_t{10}, 1000000);

BENCHMARK(BM_fast_int<std::uint64_t>)->RangeMultiplier(10)->Range(std::uint64_t{10}, 1000000);
BENCHMARK(BM_fast_int_swar<std::uint64_t>)->RangeMultiplier(10)->Range(std::int64_t{10}, 1000000);
BENCHMARK(BM_from_chars<std::uint64_t>)->RangeMultiplier(10)->Range(std::uint64_t{10}, 1000000);

BENCHMARK_MAIN();
