// bigfile: a header-only C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// Implementation of BigArchive.

#include <bigfile/big_archive.h>

// Include the implementation structure TU
#include "bigarchive_impl.cpp"

namespace bigfile {

	BigArchive::BigArchive() {
		impl = new BigArchive::Impl();
	}

	BigArchive::~BigArchive() {
		delete impl;
	}

	bool BigArchive::ReadFrom(std::istream& stream) {
		if(!stream)
			return false;

		byte magic[4];

		// read in the header
		stream.read((char*)&magic, 4);

		auto format = GetArchiveType(magic);

		switch(format) {
			case ArchiveType::BIGF:
				// read a BIGF bigfile
				impl->ReadBigfImpl(stream);
				break;

			case ArchiveType::CoFb:
				// seek back to after 0xC0FB
				stream.seekg(2, std::istream::beg);
				impl->ReadCofbImpl(stream);
				return true;
				break;

			case ArchiveType::NotArchive:
			default:
				return false;
				break;
		}

		return true;
	}


	ArchiveType BigArchive::GetCurrentArchiveType() {
		return impl->type;
	}

	std::optional<File> BigArchive::GetFile(const std::string& path) {
		return impl->GetFileImpl(path);
	}

	std::vector<std::string> BigArchive::GetPaths() {
		std::vector<std::string> temp;

		for(auto&& t : impl->files)
			temp.push_back(t.first);

		return temp;
	}

} // namespace bigfile
