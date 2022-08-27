# Main test executable; depends on Google Benchmark, requires C++17 support
# since this benchmarks `std::from_chars` as well.
add_executable(benchmarks benchmark.cpp)
target_compile_features(benchmarks PRIVATE cxx_std_17)
target_link_libraries(benchmarks
  PRIVATE
  benchmark::benchmark
  fast_int
)

target_compile_options(benchmarks
  PRIVATE
  -g
)
