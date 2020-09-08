## bigfile

A C++ library for reading data from Electronic Arts BIGF and COFB archives. 

Also provides code for working with EA RefPack compressed data.


## Building

### Requirements

  * A standards-compliant C++17 compiler (Even MSVC will work)
  * Git


### CMake Command line

```
cmake .. -DBIGFILE_ENABLE_EXAMPLES=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Using 

Usage examples (Sample programs) are available in the src/examples directory.
