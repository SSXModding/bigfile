//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

// example big information program using bigfile

#include <bigfile.h>

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

// Format a raw amount of bytes to a human-readable unit.
std::string FormatUnit(std::uint64_t bytes) {
	char buf[1024];
	constexpr auto unit = 1024;

	int exp = 0;
	int div = unit;

	if(bytes < unit) {
		sprintf(buf, "%llu B", bytes);
		return buf;
	} else {
		for(int i = bytes / unit; i >= unit; i /= unit) {
			div *= unit;
			exp++;
		}
	}

	sprintf(buf, "%0.2f %cB", float(bytes)/float(div), "kMG"[exp]);
	return buf;
}

int main(int argc, char** argv) {
	if(argc < 2) {
		std::cout << "Usage: " << argv[0] << " [.big file]\n";
		std::cout << "biginfo is a program for retrieving information\n";
		std::cout << "about a EA .BIG archive file.\n";
		return 1;
	}

	std::string file = argv[1];
	std::ifstream stream(file, std::ifstream::binary);

	bigfile::BigArchive archive;

	if(!archive.ReadToc(stream)) {
		std::cout << "Error reading BIG file.\n";
		return 1;
	}

	std::cout << file << ": ";

	switch(archive.GetArchiveType()) {
		case bigfile::ArchiveType::BIGF:
			std::cout << "BIGF (old 32-bit), ";
			break;

		case bigfile::ArchiveType::C0FB:
			std::cout << "C0FB (old 24-bit), ";
			break;

		default:
			break;
	}

	switch(archive.GetPackType()) {
		case bigfile::PackType::Uncompressed:
			std::cout << "not packed\n";
			break;

		case bigfile::PackType::RefPack:
			std::cout << "RefPack packed\n";
			break;

		default:
			break;
	}

	// If archive has debug information, print it out...
	if(auto di = archive.GetDebugInfo(); di.has_value()) {
		auto& dinfo = *di;
		std::cout << "Debug Info (Packer version, flags...):\n";

		std::cout << "\t\b\b\b\bLumpy version: " << dinfo.LumpyVersion[1] << '.' << dinfo.LumpyVersion[2] << dinfo.LumpyVersion[3] << '\n';
		std::cout << "\t\b\b\b\bLumpy flags: " << std::hex << dinfo.LumpyFlags << std::dec << '\n';
	}

	std::cout << "Files:\n";

	auto paths = archive.GetPaths();
	for(std::string& path : paths) {
		auto& bigFile = archive.GetFile(path, false).value().get();
		auto sizeStr = FormatUnit(bigFile.size);

		if(archive.GetPackType() == bigfile::PackType::RefPack) {
			auto compressedSizeStr = FormatUnit(bigFile.compressed_size);
			std::cout << std::setw(4) << "\"" << path << "\" size " << sizeStr << " (in-archive size " << compressedSizeStr << ")" << std::endl;
		} else {
			std::cout << std::setw(4) << "\"" << path << "\" size " << sizeStr << std::endl;
		}
	}
	return 0;
}
