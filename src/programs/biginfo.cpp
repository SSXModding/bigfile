//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

// big information program using bigfile

#include <bigfile/BigArchive.h>

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

/**
 * Format a raw amount of bytes to a human-readable unit.
 * \param[in] bytes Size in bytes.
 */
std::string FormatUnit(std::uint64_t bytes) {
	char buf[1024];
	constexpr auto unit = 1024;

	std::size_t exp{};
	std::size_t div = unit;

	if(bytes < unit) {
		sprintf(buf, "%zu B", bytes);
		return buf;
	} else {
		for(std::uint64_t i = bytes / unit; i >= unit; i /= unit) {
			div *= unit;
			exp++; // TODO: break if too big
		}
	}

#define CHECKED_LIT(literal, expression) (literal)[std::clamp(expression, std::size_t(0), sizeof(literal) - 1)]
	sprintf(buf, "%0.2f %cB", float(bytes)/float(div), CHECKED_LIT("kMG", exp));
#undef CHECKED_LIT
	return buf;
}

int main(int argc, char** argv) {
	if(argc < 2) {
		std::cout << "biginfo is a program for dumping information\n";
		std::cout << "about a EA .BIG archive file.\n";
		std::cout << "Usage: " << argv[0] << " [.big file]\n";
		return 1;
	}

	std::string file = argv[1];
	std::ifstream stream(file, std::ifstream::binary);

	bigfile::BigArchive archive;

	if(!archive.ReadArchive(stream)) {
		std::cout << "Error reading BIG file.\n";
		return 1;
	}

	// it might be nice to offer json output as well, just for debugging/standardization
	// also a cli creation tool that takes json output from bigextract (with an optional param) would be nice.

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
			std::cout << "RefPack\n";
			break;

		// TODO: other packtypes. ssx only uses refpack

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

	for(const auto& [path, bigfile] : archive) {
		auto sizeStr = FormatUnit(bigfile.GetSize());

		if(archive.GetPackType() == bigfile::PackType::RefPack) {
			auto compressedSizeStr = FormatUnit(bigfile.GetCompressedSize());
			std::cout << std::setw(4) << "\"" << path << "\" size " << sizeStr << " (in-archive size " << compressedSizeStr << ")" << std::endl;
		} else {
			std::cout << std::setw(4) << "\"" << path << "\" size " << sizeStr << std::endl;
		}
	}
	return 0;
}
