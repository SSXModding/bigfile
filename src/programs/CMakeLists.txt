add_executable(bigextract bigextract.cpp)
set_target_properties(bigextract PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED ON
		)
target_link_libraries(bigextract bigfile)

add_executable(biginfo biginfo.cpp)
set_target_properties(biginfo PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED ON
		)
target_link_libraries(biginfo bigfile)