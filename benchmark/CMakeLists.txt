find_package(benchmark REQUIRED)
find_package(FastFloat REQUIRED)

# Main test executable; depends on Google Benchmark and `fast_float`, requires
# C++17 support since this benchmarks `std::from_chars` as well.
add_executable(benchmarks benchmark.cpp)
target_compile_features(benchmarks PRIVATE cxx_std_17)
target_link_libraries(benchmarks
  PRIVATE
  benchmark::benchmark
  fast_int
  FastFloat::fast_float
)
target_compile_options(benchmarks
  PRIVATE
  -fno-omit-frame-pointer
)
