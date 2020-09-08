// Example program that extracts all files in an archive using bigfile.

#include <iostream>
// std::quoted
#include <iomanip>
#include <vector>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

#include <bigfile.h>

int main(int argc, char** argv) {
	if(argc < 3) {
		std::cout << "Usage: " << argv[0] << " [.big file] [output directory]\n";
		return 1;
	}

	fs::path root(argv[2]);
	std::ifstream stream(argv[1], std::ifstream::binary);

	bigfile::BigArchive archive;

	if(!archive.ReadFrom(stream)) {
		std::cout << "Error reading archive\n";
	}

	switch(archive.GetCurrentArchiveType()) {
		case bigfile::ArchiveType::BIGF:
			std::cout << "BIGF archive\n";
			break;

		case bigfile::ArchiveType::CoFb:
			std::cout << "SSX Tricky C0FB archive\n";
			break;

		default:
			break;
	}

	if(!fs::exists(root))
		fs::create_directory(root);

	std::vector<std::string> paths = archive.GetPaths();

	for(std::string path : paths) {
		auto file = archive.GetFile(path);

		// normalize path if the file name contains one,
		// and create the directory tree too
		if(path.find('/') != std::string::npos || path.find('\\') != std::string::npos) {
			while(true) {
				auto it = path.find('/');

				if(it != std::string::npos)
					path[it] = fs::path::preferred_separator;
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
		stream.write((char*)file->data.data(), file->data.size());
		stream.close();
	}

	std::cout << "Done extracting files.\n";

	return 0;
}
