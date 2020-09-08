// bigfile: a header-only C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// Example program that gets all paths in an archive using bigfile.

#include <iostream>
#include <vector>
#include <fstream>
#include <bigfile.h>

int main(int argc, char** argv) {
	if(argc < 2) {
		std::cout << "Usage: " << argv[0] << " [.big file]\n";
		return 1;
	}

	std::string file = argv[1];
	std::ifstream stream(file, std::ifstream::binary);

	bigfile::BigArchive archive;

	if(!archive.ReadFrom(stream)) {
		std::cout << "Error reading archive\n";
	}

	std::vector<std::string> paths = archive.GetPaths();

	for(std::string path : paths)
		std::cout << path << "\n";

	return 0;
}
