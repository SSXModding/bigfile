## bigfile

A C++ library for reading data from Electronic Arts BIG archives.

Usage examples are available in the src/examples directory.


## Building

### Requirements

  * A standards-compliant C++17 compiler
  * Git


### CMake Command line

```
cmake .. -DBIGFILE_ENABLE_EXAMPLES=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .
```