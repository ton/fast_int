#include <benchmark/benchmark.h>
#include <fast_int/fast_int.hpp>

#ifdef WITH_FAST_FLOAT
#include <fast_float/fast_float.h>
#endif

#include <charconv>
#include <cstdint>
#include <cstring>

template<typename T>
static void BM_fast_int(benchmark::State &state)
{
  const std::string s{std::to_string(state.range(0))};

  for (auto _ : state)
  {
    T i{};
    fast_int::from_chars(s.data(), s.data() + s.size(), i);
    benchmark::DoNotOptimize(i);
  }
}

template<typename T>
static void BM_strtol(benchmark::State &state)
{
  const std::string s{std::to_string(state.range(0))};

  for (auto _ : state)
  {
    T i{};
    std::strtol(s.data(), nullptr, 10);
    benchmark::DoNotOptimize(i);
  }
}

#ifdef WITH_FAST_FLOAT
template<typename T>
static void BM_fast_float(benchmark::State &state)
{
  const std::string s{std::to_string(state.range(0))};

  for (auto _ : state)
  {
    double i{};
    fast_float::from_chars(s.data(), s.data() + s.size(), i);
    benchmark::DoNotOptimize(i);
  }
}
#endif

template<typename T>
static void BM_from_chars(benchmark::State &state)
{
  const std::string s{std::to_string(state.range(0))};

  for (auto _ : state)
  {
    T i{};
    std::from_chars(s.data(), s.data() + s.size(), i);
    benchmark::DoNotOptimize(i);
  }
}

BENCHMARK(BM_fast_int<std::int64_t>)->RangeMultiplier(10)->Range(std::int64_t{1}, 1000000);
BENCHMARK(BM_strtol<std::int64_t>)->RangeMultiplier(10)->Range(std::int64_t{1}, 1000000);
BENCHMARK(BM_from_chars<std::int64_t>)->RangeMultiplier(10)->Range(std::int64_t{1}, 1000000);

#ifdef WITH_FAST_FLOAT
BENCHMARK(BM_fast_float<std::int64_t>)->RangeMultiplier(10)->Range(std::int64_t{1}, 1000000);
#endif

BENCHMARK(BM_fast_int<std::uint64_t>)->RangeMultiplier(10)->Range(std::uint64_t{1}, 1000000);
BENCHMARK(BM_strtol<std::uint64_t>)->RangeMultiplier(10)->Range(std::uint64_t{1}, 1000000);
BENCHMARK(BM_from_chars<std::uint64_t>)->RangeMultiplier(10)->Range(std::uint64_t{1}, 1000000);

#ifdef WITH_FAST_FLOAT
BENCHMARK(BM_fast_float<std::uint64_t>)->RangeMultiplier(10)->Range(std::uint64_t{1}, 1000000);
#endif

BENCHMARK_MAIN();
