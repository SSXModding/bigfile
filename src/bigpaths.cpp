// bigfile: a header-only C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// Example program that gets all paths in an archive using bigfile.

#include <iostream>
#include <fstream>
#include "bigfile.hpp"

int main(int argc, char** argv) {
	if(argc < 2) {
		std::cout << "Usage: " << argv[0] << " [.big file]\n";
		return 1;
	}

	std::string file = argv[1];
	std::ifstream stream(file);

	bigfile::BigArchive archive(stream);

	std::vector<std::string> paths = archive.GetAllPaths();

	for(std::string path : paths)
		std::cout << path << "\n";

	return 0;
}
