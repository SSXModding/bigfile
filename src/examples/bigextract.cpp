// bigfile: a header-only C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// Example program that extracts all files in an archive using bigfile.

#include <iostream>
#include <vector>
#include <fstream>
#include "bigfile.hpp"

int main(int argc, char** argv) {
	if(argc < 2) {
		std::cout << "Usage: " << argv[0] << " [.big file]\n";
		return 1;
	}

	std::string big = argv[1];
	std::ifstream stream(big, std::ifstream::binary);

	bigfile::BigArchive archive(stream);

	std::vector<std::string> paths = archive.GetPaths();

	for(std::string path : paths) {
		std::cout << path << '\n';

		// NOTE:
		//	We do no handling of if GetFile() returns BigFile::Empty because we most likely
		//	have paths that are all valid.
		//	For a program expecting a user or developer-specified path instead of 
		//	paths directly from the file table, it's reccomended that you instead do something like:
		//
		//	bigfile::BigArchive archive((... stream object of your liking))
		//	... do checking code to make sure the archive loaded in ...
		//
		//	auto file = archive.GetFile("path/here/thing.txt");
		//	if(!file)
		//		... code to invoke on file not existing ...
		//	... do what you like with the file ...
		//
		auto file = archive.GetFile(path);

		std::ofstream stream(path, std::ofstream::binary);
		stream << file.data.data();
		stream.close();

		// NOTE:
		//	A cleanup stage is no longer required!
		//	Because GetFile() returns a reference to what's stored in the map of the BigArchive instance
		//	instead of a copy, all cleanup will be done when the archive object is destructed.
	}

	return 0;
}
