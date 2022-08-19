#include <benchmark/benchmark.h>
#include <fast_int/fast_int.hpp>

#ifdef WITH_FAST_FLOAT
#include <fast_float/fast_float.h>
#endif

#include <charconv>
#include <cstdint>
#include <cstring>

template<typename T>
constexpr const char *input();

template<>
constexpr const char *input<std::int32_t>()
{
  return "-12345";
}

template<>
constexpr const char *input<std::int64_t>()
{
  return "-223372036854775808";
}

template<>
constexpr const char *input<std::uint64_t>()
{
  return "9223372036854775808";
}

template<typename T>
static void BM_fast_int(benchmark::State &state)
{
  const char *number{input<T>()};
  const char *const last{number + std::strlen(number)};
  T sum{};

  for (auto _ : state)
  {
    T i{};
    fast_int::from_chars(number, last, i);
    benchmark::DoNotOptimize(sum += i);
  }
}

template<typename T>
static void BM_strtol(benchmark::State &state)
{
  const char *number{input<T>()};
  T sum{};

  for (auto _ : state)
  {
    T i{};
    std::strtol(number, nullptr, 10);
    benchmark::DoNotOptimize(sum += i);
  }
}

template<typename T>
static void BM_atol(benchmark::State &state)
{
  const char *number{input<T>()};
  T sum{};

  for (auto _ : state)
  {
    T i = std::atol(number);
    benchmark::DoNotOptimize(sum += i);
  }
}

#ifdef WITH_FAST_FLOAT
template<typename T>
static void BM_fast_float(benchmark::State &state)
{
  const char *number{input<T>()};
  const char *last{number + strlen(number)};
  double sum{};

  for (auto _ : state)
  {
    double i{};
    fast_float::from_chars(number, last, i);
    benchmark::DoNotOptimize(sum += i);
  }
}
#endif

template<typename T>
static void BM_from_chars(benchmark::State &state)
{
  const char *number{input<T>()};
  const char *const last{number + std::strlen(number)};
  T sum{};

  for (auto _ : state)
  {
    T i{};
    std::from_chars(number, last, i);
    benchmark::DoNotOptimize(sum += i);
  }
}

BENCHMARK(BM_fast_int<std::int32_t>);
BENCHMARK(BM_from_chars<std::int32_t>);
BENCHMARK(BM_strtol<std::int32_t>);
BENCHMARK(BM_atol<std::int32_t>);
#ifdef WITH_FAST_FLOAT
BENCHMARK(BM_fast_float<std::int32_t>);
#endif

BENCHMARK(BM_fast_int<std::int64_t>);
BENCHMARK(BM_from_chars<std::int64_t>);
BENCHMARK(BM_strtol<std::int64_t>);
BENCHMARK(BM_atol<std::int64_t>);
#ifdef WITH_FAST_FLOAT
BENCHMARK(BM_fast_float<std::int64_t>);
#endif

BENCHMARK(BM_fast_int<std::uint64_t>);
BENCHMARK(BM_from_chars<std::uint64_t>);
BENCHMARK(BM_strtol<std::uint64_t>);
BENCHMARK(BM_atol<std::uint64_t>);
#ifdef WITH_FAST_FLOAT
BENCHMARK(BM_fast_float<std::uint64_t>);
#endif

BENCHMARK_MAIN();
