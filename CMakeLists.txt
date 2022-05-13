cmake_minimum_required(VERSION 3.1.0)
project(bigfile CXX)

option(BIGFILE_ENABLE_EXAMPLES "Enable compiling of examples of how to use Bigfile" ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()

add_subdirectory(src/bigfile)

if(BIGFILE_ENABLE_EXAMPLES)
	add_subdirectory(src/examples)
endif()