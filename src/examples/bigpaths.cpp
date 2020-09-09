// example ls-like program using bigfile

#include <iostream>
#include <vector>
#include <fstream>
#include <bigfile.h>

int main(int argc, char** argv) {
	if(argc < 2) {
		std::cout << "Usage: " << argv[0] << " [.big file]\n";
		return 1;
	}

	bool enable_cofb = false;

	std::string file = argv[1];
	std::ifstream stream(file, std::ifstream::binary);

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
			enable_cofb = true;
			break;

		default:
			break;
	}

	std::vector<std::string> paths = archive.GetPaths();
	std::vector<bigfile::File> files;

	for(std::string path : paths) {
		auto file = archive.GetFile(path).value();

		if(enable_cofb) {
			if(file.type == bigfile::CofbFileType::Refpack)
				std::cout << "\"" << path << "\" (compressed) size " << file.compressed_size / 1000 << " KB (actual size " << file.size / 1000 << " KB)\n";
			else
				std::cout << "\"" << path << "\" (uncompressed) size " << file.size / 1000 << " KB\n";
		} else {
			std::cout << "\"" << path << "\" (uncompressed) size " << file.size / 1000 << " KB\n";
		}
	}
	return 0;
}
