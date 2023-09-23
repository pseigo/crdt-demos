# CRDT Client (C++)

> **Note**: This is a work in progress!

## How to Run It

Install [CMake](https://cmake.org/download/) and a C++ compiler.

```sh
# (Assuming you are in the `client-cpp` directory.)

rm -rf build # (Deletes a previous build)
mkdir build # (The directory name is arbitrary)
cd build

# Configure and build
cmake -S .. -B .

# Run
./src/CrdtClientCpp
```

- For help with installing a C++ compiler or CMake, please search for instructions specific to your operating system distribution. Some popular compilers are [Clang](https://clang.llvm.org), [GCC](https://gcc.gnu.org), and [MSVC](https://learn.microsoft.com/en-us/cpp/build/reference/c-cpp-building-reference).
- For help with CMake, please reference the [CMake documentation](https://cmake.org/cmake/help/latest/index.html).
