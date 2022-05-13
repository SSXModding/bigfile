//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

// example ls-like program using bigfile

#include <bigfile.h>

#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
	if(argc < 2) {
		std::cout << "Usage: " << argv[0] << " [.big file]\n";
		return 1;
	}

	bool enable_cofb = false;

	std::string file = argv[1];
	std::ifstream stream(file, std::ifstream::binary);

	bigfile::BigArchive archive;

	if(!archive.ReadToc(stream)) {
		std::cout << "Error reading archive\n";
	}

	switch(archive.GetArchiveType()) {
		case bigfile::ArchiveType::BIGF:
			std::cout << "BIGF (Old-style 32-bit) archive\n";
			break;

		case bigfile::ArchiveType::C0FB:
			std::cout << "C0FB (Old-style 24-bit) archive\n";
			enable_cofb = true;
			break;

		default:
			break;
	}

	std::vector<std::string> paths = archive.GetPaths();

	for(std::string& path : paths) {
		// Normally we might want to use false to indicate
		// we don't want to read data, but right now we have to
		// in order to get the decompressed size.
		//
		// This is a bit of a TODO.
		auto& bigFile = archive.GetFile(path).value().get();

		if(bigFile.type == bigfile::BigArchive::File::PackType::Refpack)
			std::cout << "\"" << path << "\" (RefPacked) size " << bigFile.size / 1000 << " KB (stored size " << bigFile.compressed_size / 1000 << " KB)\n";
		else
			std::cout << "\"" << path << "\" size " << bigFile.size / 1000 << " KB\n";

		bigFile.Done();
	}
	return 0;
}
