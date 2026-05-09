set(BENCHMARK_ENABLE_TESTING OFF)

FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark
  GIT_TAG v1.9.5
)

FetchContent_MakeAvailable(benchmark)

if(FASTINT_WITH_BENCHMARK_AGAINST_FAST_FLOAT)
  FetchContent_Declare(
    FastFloat
    GIT_REPOSITORY https://github.com/fastfloat/fast_float
    GIT_TAG v8.2.5
  )

  FetchContent_MakeAvailable(FastFloat)
else()
  message(STATUS "Benchmarking against fast_float has been disabled, set FASTINT_WITH_BENCHMARK_AGAINST_FAST_FLOAT to ON to enable")
endif()

# Main test executable; depends on Google Benchmark, requires C++17 support
# since this benchmarks `std::from_chars` as well.
add_executable(benchmarks benchmark.cpp)
target_compile_features(benchmarks PRIVATE cxx_std_17)
target_link_libraries(benchmarks
  PRIVATE
    benchmark::benchmark
    $<$<TARGET_EXISTS:FastFloat::fast_float>:FastFloat::fast_float>
    fast_int
    fast_int_internal_compiler_flags
)

target_compile_options(benchmarks
  PRIVATE
    -g
)

target_compile_definitions(benchmarks
  PRIVATE
    $<$<TARGET_EXISTS:FastFloat::fast_float>:HAS_FAST_FLOAT>
)
