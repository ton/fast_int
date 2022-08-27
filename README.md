fast_int
--------

The `fast_int` library provides header-only C++11 implementations for the C++ `from_chars` functions for integer types. Performance of `fast_int` is at least as good as `std::from_chars` on the machines tested so far.

Example
-------

Simple example that converts some text to an integer:

```C++
#include <cassert>
#include <cstdint>
#include <cstring>

const char *s{"12345"};
std::int32_t n{};
fast_int::from_chars_result result = fast_int::from_chars(s, s + std::strlen(s), n);
std::assert(!result.ec);
std::assert(n == 12345);
```

Usage
-----

Only one public function is provided by the library:

```C++
template<typename T>
from_chars_result from_chars(const char *first, const char *last, T &value) noexcept;
```

This assumes that `T` is of some integral type. In case this precondition is not met, a compiler error is issued. Another precondition is that the textual number is in base 10. Hexadecimal numbers for example are not (yet) supported.

Other than these limitations, this tries to mimic behavior of [`std::from_chars`](https://en.cppreference.com/w/cpp/utility/from_chars) as closely as possible for integral types.

The result structure `fast_int::from_chars_result` is similar to that of `std::from_chars_result`. Please refer to the documentation of [`std::from_chars`](https://en.cppreference.com/w/cpp/utility/from_chars) for more details.

Benchmark results
-----------------

The benchmarks are somewhat limited, but the results give some impression on the performance of this library relative to other solutions.

```
2022-08-24T21:39:09+02:00
Running build/benchmarks
Run on (12 X 3600 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 512 KiB (x6)
  L3 Unified 16384 KiB (x2)
Load Average: 0.45, 0.41, 0.27
-------------------------------------------------------------------------------
Benchmark                                     Time             CPU   Iterations
-------------------------------------------------------------------------------
BM_fast_int<std::int64_t>/100000           4898 ns         4896 ns       142309
BM_fast_int<std::int64_t>/1000000          6034 ns         6032 ns       114400
BM_from_chars<std::int64_t>/100000         7204 ns         7202 ns        98529
BM_from_chars<std::int64_t>/1000000        8121 ns         8119 ns        85194
BM_fast_int<std::uint64_t>/100000          4343 ns         4341 ns       163451
BM_fast_int<std::uint64_t>/1000000         4998 ns         4997 ns       138293
BM_from_chars<std::uint64_t>/100000        4737 ns         4736 ns       147341
BM_from_chars<std::uint64_t>/1000000       5570 ns         5568 ns       125368
```

Dependencies
------------

This library has no external dependencies. To be able to use the library, the simplest approach is to just drop `fast_int.hpp` into your project directly. To be able to install the header files and CMake configuration files, the following dependencies are required:

* [CMake](https://cmake.org/)
* [ninja](https://ninja-build.org/)

To be able to build the unit tests, the following dependency is required:

* [Catch2](https://github.com/catchorg/Catch2)

To be able to build the benchmarks, the following dependency is required:

* [Benchmark](https://github.com/google/benchmark)

In case these dependencies are not detected by CMake, building the unit tests and/or benchmarks is automatically disabled.

Installation
------------

The `fast_int` library is a header-only library. The simplest way to include it in your project is to directly drop `fast_int.hpp` in your project. An installation target is provided by the CMake files to be able to install the header in a location of your choice. A separate `configure` script is provided to make it a bit more convenient to execute CMake. For example, to install the header files in your `$HOME/.local` directory, issue:

```
$ ./configure --prefix=$HOME/local
$ ninja -C build install
```

To build the unit tests and benchmarks in release mode:

```
$ ./configure --build-type=release
$ ninja -C build
```

CMake
-----

Installing this library will automatically install CMake configuration files that allow you to find `fast_int` from other projects:

```cmake
find_package(fast_int REQUIRED)
```

This will add a header-only library target `fast_int::fast_int` you can link to for targets that depend on this library.

Limitations
-----------

Only base 10 integers are supported for now.

Credit
------

This implementation was almost one-to-one provided by Daniel Lemire in the issue tracker of the [`fast_float`](https://github.com/fastfloat/fast_float) library, in particular [issue #86](https://github.com/fastfloat/fast_float/issues/86), where someone requests `fast_int` functionality for the [`fast_float`](https://github.com/fastfloat/fast_float) library. I intend to get this functionality merged into the [`fast_float`](https://github.com/fastfloat/fast_float) library at some point, but at this moment I consider the code to be too immature, it needs more rigorous review.

License
-------

Licensed under the MIT license.
