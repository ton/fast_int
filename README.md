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

The benchmarks are somewhat limited, but the results give some impression on the performance of this library relative to other solutions. Note that `fast_float` right now cannot be used to convert textual numbers to integers, it is included in the benchmarks merely for illustrative purposes.

```
2022-08-17T13:26:59+02:00
Running build/benchmarks
Run on (12 X 4208.2 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 512 KiB (x6)
  L3 Unified 16384 KiB (x2)
Load Average: 0.34, 0.18, 0.13
----------------------------------------------------------------------
Benchmark                            Time             CPU   Iterations
----------------------------------------------------------------------
BM_fast_int<std::int32_t>        0.239 ns        0.239 ns   1000000000
BM_fast_int<std::int64_t>         13.1 ns         13.1 ns     53546725
BM_strtol<std::int32_t>           10.3 ns         10.3 ns     68603272
BM_strtol<std::int64_t>           27.2 ns         27.2 ns     26654442
BM_fast_float<std::int32_t>       8.65 ns         8.65 ns     80912265
BM_fast_float<std::int64_t>       13.3 ns         13.3 ns     52397382
BM_from_chars<std::int32_t>      0.314 ns        0.314 ns   1000000000
BM_from_chars<std::int64_t>       14.1 ns         14.1 ns     49426511
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

In case these dependencies are not detected by CMake, building the unit tests and/or benchmarks is automatically disabled. Additionally, for the benchmark tool, there is one additional optional dependency which when detected will enable some additional benchmarks:

* [fast_float](https://github.com/fastfloat/fast_float)

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
