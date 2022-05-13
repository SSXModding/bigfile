add_executable(bigextract ${PROJECT_SOURCE_DIR}/src/examples/bigextract.cpp)
set_property(TARGET bigextract PROPERTY CXX_STANDARD 20)
set_property(TARGET bigextract PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(bigextract bigfile)

add_executable(bigpaths ${PROJECT_SOURCE_DIR}/src/examples/bigpaths.cpp)
set_property(TARGET bigpaths PROPERTY CXX_STANDARD 20)
set_property(TARGET bigpaths PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(bigpaths bigfile)