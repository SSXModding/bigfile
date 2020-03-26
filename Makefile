CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

all: bigpaths

bigpaths: src/bigpaths.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f bigpaths
