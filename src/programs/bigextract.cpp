//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

// Example program that extracts all files in an archive using bigfile.

#include <iostream>
// std::quoted
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <vector>

namespace fs = std::filesystem;

#include <bigfile/BigArchive.h>

int main(int argc, char** argv) {
	if(argc < 3) {
		std::cout << "Usage: " << argv[0] << " [.big file] [output directory]\n";
		return 1;
	}

	fs::path root(argv[2]);
	std::ifstream stream(argv[1], std::ifstream::binary);

	bigfile::BigArchive archive;

	if(!archive.ReadArchive(stream)) {
		std::cout << "Error reading archive\n";
		return 1;
	}

	switch(archive.GetArchiveType()) {
		case bigfile::ArchiveType::BIGF:
			std::cout << "Extracting a BIGF (old style 32-bit) archive\n";
			break;

		case bigfile::ArchiveType::C0FB:
			std::cout << "Extracting a C0FB (old-style 16-bit) archive\n";
			break;

		default:
			break;
	}

	if(!fs::exists(root))
		fs::create_directory(root);


	for(auto& [path, file] : archive) {
		// This is a bit screwy, on Windows it won't convert / paths
		// (which is fine, I think?) but on Linux it will convert \ paths.
		// I wish I knew why. Hopefully this doesn't break
		auto fixedup_path = fs::path(root / path).make_preferred();

		if(!file.ReadFile()) {
			std::cout << "Couldn't read file data?\n";
			return 1;
		}

		// create directory tree
		fs::create_directories(fixedup_path.parent_path());

		std::cout << "Extracting file \"" << fixedup_path.string() << "\"...\n";

		std::ofstream stream(fixedup_path.string(), std::ofstream::binary);

		stream.write(reinterpret_cast<const char*>(file.GetData().data()), static_cast<std::streamsize>(file.GetData().size()));
		stream.close();

		file.Done();
	}

	std::cout << "Done extracting files.\n";

	return 0;
}
