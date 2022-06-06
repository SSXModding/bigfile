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
			std::cout << "Extracting a SSX Tricky C0FB archive\n";
			break;

		default:
			break;
	}

	if(!fs::exists(root))
		fs::create_directory(root);


	for(auto& [path, file] : archive) {
		std::string fixedup_path = path;

		if(!file.ReadFile()) {
			std::cout << "Couldn't read file data?\n";
			return 1;
		}

		// normalize path if the file name contains one,
		// and create the directory tree too
		if(fixedup_path.find('/') != std::string::npos || fixedup_path.find('\\') != std::string::npos) {
			while(true) {
				auto it = fixedup_path.find('/');
				if(it == std::string::npos)
					it = fixedup_path.find('\\');

				if(it != std::string::npos)
					if constexpr (fs::path::preferred_separator == '/')
						break;
					else
						fixedup_path[it] = fs::path::preferred_separator;
				else
					break;
			}

			auto nofile = path.substr(0, path.rfind(fs::path::preferred_separator));

			// create directory tree
			fs::create_directories(root / nofile);
		}

		auto fspath = root / path;

		std::cout << "Extracting file \"" << fspath.string() << "\"...\n";

		std::ofstream stream(fspath.string(), std::ofstream::binary);

		stream.write(reinterpret_cast<const char*>(file.GetData().data()), file.GetData().size());
		stream.close();

		file.Done();
	}

	std::cout << "Done extracting files.\n";

	return 0;
}
