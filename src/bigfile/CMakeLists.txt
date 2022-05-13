
add_library(bigfile STATIC
		BigArchive.cpp
		RefPack.cpp
		ops/ReadFileOp.cpp
		ops/ReadHeaderAndTocOp.cpp
		)

set_property(TARGET bigfile PROPERTY CXX_STANDARD 20)
set_property(TARGET bigfile PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(bigfile PUBLIC ${PROJECT_SOURCE_DIR}/include)
