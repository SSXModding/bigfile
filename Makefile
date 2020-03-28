CXX = g++
AR = ar
CXXFLAGS = -std=c++17 -Iinclude

PREFIX = /usr/include/

BIGFILE_INCLUDE_DIR = include
BIGFILE_PATHS = $(BIGFILE_INCLUDE_DIR)/bigfile.hpp \
                $(BIGFILE_INCLUDE_DIR)/bigfile/big_archive.hpp \
                $(BIGFILE_INCLUDE_DIR)/bigfile/structures.hpp \
                $(BIGFILE_INCLUDE_DIR)/bigfile/utility_functions.hpp \
                $(BIGFILE_INCLUDE_DIR)/bigfile/impl/big_archive.hpp \
                $(BIGFILE_INCLUDE_DIR)/bigfile/impl/structures.hpp \
                $(BIGFILE_INCLUDE_DIR)/bigfile/impl/utility_functions.hpp

all: bigpaths bigextract libbigfile.a libbigfile.so

bigpaths: src/bigpaths.cpp $(BIGFILE_PATHS)
	$(CXX) $(CXXFLAGS) $< -o $@

bigextract: src/bigextract.cpp $(BIGFILE_PATHS)
	$(CXX) $(CXXFLAGS) $< -o $@

libbigfile.a: src/compile_as_library/bigfile.cpp $(BIGFILE_PATHS)
	$(CXX) $(CXXFLAGS) -c $< -o bigfile.o
	$(AR) rv $@ bigfile.o
	rm bigfile.o

libbigfile.so: src/compile_as_library/bigfile.cpp $(BIGFILE_PATHS)
	$(CXX) $(CXXFLAGS) -fPIC -shared $< -o $@

clean:
	rm -f bigpaths
	rm -f bigextract
	rm -f libbigfile.a
	rm -f libbigfile.so

install:
	-mkdir $(PREFIX)/bigfile
	-cp -rv $(BIGFILE_INCLUDE_DIR)/* $(PREFIX)/bigfile
