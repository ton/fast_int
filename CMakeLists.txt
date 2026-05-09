cmake_minimum_required(VERSION 3.14)

project(fast_int
  VERSION 0.0.1
  LANGUAGES CXX
  HOMEPAGE_URL "https://github.com/ton/fast_int"
  DESCRIPTION "C++11 header-only implementation of C++17s `std::from_chars` function"
)

# Custom cmake modules.
set(PROJECT_CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_CMAKE_MODULE_PATH})

# Require C++11 support.
set(CMAKE_CXX_STANDARD 11)

# Put all executables in build/.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Define the header-only library target for fast_int.
add_library(fast_int INTERFACE)
target_include_directories(fast_int
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(fast_int INTERFACE cxx_std_11)

add_library(fast_int_internal_compiler_flags INTERFACE)

# Set the appropriate compiler flags (GCC).
target_compile_options(fast_int_internal_compiler_flags INTERFACE "$<$<CXX_COMPILER_ID:GNU>:-Wall;-Werror;-Wextra;-Wshadow;-Wstrict-overflow;-fno-strict-aliasing;-march=native>")
target_compile_options(fast_int_internal_compiler_flags INTERFACE "$<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:DEBUG>>:-g;-Wno-unused-parameter;-Wno-unused-variable;-Wno-unused-function>")
target_compile_options(fast_int_internal_compiler_flags INTERFACE "$<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:RELEASE>>:-Wunused-parameter;-Wconversion;-Wpedantic;-O3;-DNDEBUG>")

# Set the appropriate compiler flags (MSVC).
target_compile_options(fast_int_internal_compiler_flags INTERFACE "$<$<CXX_COMPILER_ID:MSVC>:/W4;/WX;/permissive>")
target_compile_options(fast_int_internal_compiler_flags INTERFACE "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:DEBUG>>:/Od>")
target_compile_options(fast_int_internal_compiler_flags INTERFACE "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RELEASE>>:/O2>")

# Find optional dependencies for the unit tests and benchmark tool.
include(CMakeDependentOption)
include(FetchContent)

# Options supported by this build.
option(FASTINT_WITH_BENCHMARK "Enable building the benchmark binary" OFF)
cmake_dependent_option(FASTINT_WITH_BENCHMARK_AGAINST_FAST_FLOAT "Pull in fast_float as a dependency and benchmark against it" OFF "FASTINT_WITH_BENCHMARK" OFF)
option(FASTINT_WITH_TEST "Enable building the unit test binary" OFF)

# Define additional targets depending on the current configuration.
if(FASTINT_WITH_TEST)
  enable_testing()
  add_subdirectory(test)
else()
  message(STATUS "Tests are disabled, set FASTINT_WITH_TEST to ON to run tests.")
endif()

if(FASTINT_WITH_BENCHMARK)
  add_subdirectory(benchmark)
else()
  message(STATUS "Benchmarks are disabled, set FASTINT_WITH_BENCHMARK to ON to run benchmarks.")
endif()

# Add install target.
include(GNUInstallDirs)

# Add uninstall target.
if(NOT TARGET uninstall)
  configure_file(
    "${PROJECT_CMAKE_MODULE_PATH}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

# Create installation targets (because of `INSTALL_INTERFACE` above, headers are automatically included.
install(TARGETS fast_int
  EXPORT ${PROJECT_NAME}_Targets
)

# Install a CMake config and version file.
include(CMakePackageConfigHelpers)
write_basic_package_version_file("fast_intConfigVersion.cmake" VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

# Create file containing installation targets, and install it.
install(EXPORT ${PROJECT_NAME}_Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

# Install build configuration and version information to the installation directories.
install(
  FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

# Install header directory.
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/fast_int DESTINATION include)
