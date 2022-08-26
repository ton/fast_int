# Main test executable; depends on Google Benchmark and optionally on
# `fast_float`, requires C++17 support since this benchmarks `std::from_chars`
# as well.
add_executable(benchmarks benchmark.cpp)
target_compile_features(benchmarks PRIVATE cxx_std_17)
target_link_libraries(benchmarks
  PRIVATE
  benchmark::benchmark
  fast_int
)

if(TARGET FastFloat::fast_float)
  target_compile_definitions(benchmarks
    PRIVATE
    WITH_FAST_FLOAT
  )
  target_link_libraries(benchmarks
    PRIVATE
    FastFloat::fast_float
  )
endif()

target_compile_options(benchmarks
  PRIVATE
  -g
)
