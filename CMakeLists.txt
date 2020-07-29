cmake_minimum_required(VERSION 3.1.0)
project(bigfile CXX)

option(BIGFILE_ENABLE_EXAMPLES "Enable compiling of examples of how to use Bigfile" ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()


set(BIGFILE_SOURCES 
	${PROJECT_SOURCE_DIR}/src/big_archive.cpp
	${PROJECT_SOURCE_DIR}/src/structures.cpp
	${PROJECT_SOURCE_DIR}/src/utility_functions.cpp
)

add_library(bigfile STATIC ${BIGFILE_SOURCES})
set_property(TARGET bigfile PROPERTY CXX_STANDARD 17)
set_property(TARGET bigfile PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(bigfile PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(BIGFILE_ENABLE_EXAMPLES)
# add examples
	add_executable(bigextract ${PROJECT_SOURCE_DIR}/src/examples/bigextract.cpp)
	set_property(TARGET bigextract PROPERTY CXX_STANDARD 17)
	set_property(TARGET bigextract PROPERTY CXX_STANDARD_REQUIRED ON)
	target_link_libraries(bigextract bigfile)
	
	add_executable(bigpaths ${PROJECT_SOURCE_DIR}/src/examples/bigpaths.cpp)
	set_property(TARGET bigpaths PROPERTY CXX_STANDARD 17)
	set_property(TARGET bigpaths PROPERTY CXX_STANDARD_REQUIRED ON)
	target_link_libraries(bigpaths bigfile)
endif()