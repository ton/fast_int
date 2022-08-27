cmake_minimum_required(VERSION 3.0)

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
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set the appropriate compiler flags.
add_compile_options("$<$<CONFIG:DEBUG>:-Wall;-Wextra;-Wshadow;-Wno-unused-variable;-Wno-unused-function;-Wno-unused-parameter;-Werror;-Wstrict-overflow;-fno-strict-aliasing;-march=native>")
add_compile_options("$<$<CONFIG:RELEASE>:-Wall;-Wextra;-Wshadow;-Wpedantic;-Werror;-Wno-unused-parameter;-Wstrict-overflow;-fno-strict-aliasing;-march=native>")

# Define the header-only library target for fast_int.
add_library(fast_int INTERFACE)
target_include_directories(fast_int
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(fast_int INTERFACE cxx_std_11)

# Find optional dependencies for the unit tests and benchmark tool.
include(FeatureSummary)

find_package(Catch2 QUIET)
set_package_properties(Catch2 PROPERTIES TYPE DESCRIPTION "C++ test framework for unit-tests.")
set_package_properties(Catch2 PROPERTIES TYPE OPTIONAL PURPOSE "Allows building the unit tests for this library.")

find_package(benchmark QUIET)
set_package_properties(benchmark PROPERTIES TYPE DESCRIPTION "a microbenchmark support library.")
set_package_properties(benchmark PROPERTIES TYPE OPTIONAL PURPOSE "Allows building the benchmarks for this library.")

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES)

# Define additional targets depending whether the dependencies have been found.
if(TARGET Catch2::Catch2WithMain)
  add_subdirectory(test)
endif()

if(TARGET benchmark::benchmark)
  add_subdirectory(benchmark)
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
