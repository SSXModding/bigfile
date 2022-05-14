add_executable(bigextract bigextract.cpp)
set_property(TARGET bigextract PROPERTY CXX_STANDARD 20)
set_property(TARGET bigextract PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(bigextract bigfile)

add_executable(biginfo biginfo.cpp)
set_property(TARGET biginfo PROPERTY CXX_STANDARD 20)
set_property(TARGET biginfo PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(biginfo bigfile)