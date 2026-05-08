FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2
  GIT_TAG v3.14.0
)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

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

catch_discover_tests(tests)
