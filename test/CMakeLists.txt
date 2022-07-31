find_package(Catch2 3 REQUIRED)

# Main test executable; depends on Catch2, requires C++14 support.
add_executable(tests main.cpp test.cpp)
target_link_libraries(tests
  PRIVATE
  Catch2::Catch2WithMain
  fast_int
)

# Executables that are only compiled to test compatibility for the various C++
# standards.
add_executable(test_cpp11 test_cpp11.cpp)
target_link_libraries(test_cpp11
  PRIVATE
  fast_int
)
