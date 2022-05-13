cmake_minimum_required(VERSION 3.1.0)
project(bigfile CXX)

include(CheckCXXSourceCompiles)

option(BIGFILE_ENABLE_EXAMPLES "Enable compiling of examples of how to use Bigfile" ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()


# Detect the particular C++ standard library if it's not cached.
if(NOT DEFINED CACHE{_BIGFILE_STDLIB})
	message(STATUS "Detecting C++ standard library")
	set(CMAKE_REQUIRED_QUIET ON)

	check_cxx_source_compiles(
		"#include <ciso646>
		#if !defined(__GLIBCXX__)
		#error \"not libstdc++\"
		#endif
		int main() {}"
	__LIBRARY_LIBSTDCXX)

	check_cxx_source_compiles(
		"#include <ciso646>
		#if !defined(_LIBCPP_VERSION)
		#error \"not libc++\"
		#endif
		int main() {}"
	__LIBRARY_LIBCXX)

	check_cxx_source_compiles(
		"#include <ciso646>
		#if !defined(_MSVC_STL_VERSION)
		#error \"not MSVC\"
		#endif
		int main() {}"
	__LIBRARY_MSVC)

	if(__LIBRARY_LIBSTDCXX)
		set(STANDARD_LIBRARY "libstdc++")
	elseif(__LIBRARY_LIBCXX)
		set(STANDARD_LIBRARY "libc++")
	elseif(__LIBRARY_MSVC)
		set(STANDARD_LIBRARY "msvc")
	else()
		message(FATAL_ERROR "Can't find which C++ runtime is in use")
	endif()

	set(_BIGFILE_STDLIB ${STANDARD_LIBRARY} CACHE STRING "The C++ standard library in use; possible values: libstdc++, libc++, msvc")

	message(STATUS "Detected ${EAGLE_STDLIB}")
endif()


set(BIGFILE_SOURCES
		${PROJECT_SOURCE_DIR}/src/BigArchive.cpp
		${PROJECT_SOURCE_DIR}/src/RefPack.cpp
		${PROJECT_SOURCE_DIR}/src/UtilityFunctions.cpp
)

add_library(bigfile STATIC ${BIGFILE_SOURCES})
set_property(TARGET bigfile PROPERTY CXX_STANDARD 20)
set_property(TARGET bigfile PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(bigfile PUBLIC ${PROJECT_SOURCE_DIR}/include)



if(BIGFILE_ENABLE_EXAMPLES)
# add examples
	add_executable(bigextract ${PROJECT_SOURCE_DIR}/src/examples/bigextract.cpp)
	set_property(TARGET bigextract PROPERTY CXX_STANDARD 20)
	set_property(TARGET bigextract PROPERTY CXX_STANDARD_REQUIRED ON)
	target_link_libraries(bigextract bigfile)

	if(_BIGFILE_STDLIB STREQUAL "libc++")
		target_link_libraries(bigextract c++fs)
	elseif(_BIGFILE_STDLIB STREQUAL "libstdc++")
		target_link_libraries(bigextract stdc++fs)
	endif()
	
	add_executable(bigpaths ${PROJECT_SOURCE_DIR}/src/examples/bigpaths.cpp)
	set_property(TARGET bigpaths PROPERTY CXX_STANDARD 20)
	set_property(TARGET bigpaths PROPERTY CXX_STANDARD_REQUIRED ON)
	target_link_libraries(bigpaths bigfile)
endif()