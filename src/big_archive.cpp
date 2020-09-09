#include <bigfile/big_archive.h>

// Include the implementation TU
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

		// Do a magic check
		byte magic[4];
		stream.read((char*)&magic, 4);

		ArchiveType format = GetArchiveType(magic);

		switch(format) {
			case ArchiveType::BIGF:
				impl->ReadBigfImpl(stream);
				break;

			case ArchiveType::CoFb:
				// seek back to after 0xC0FB
				// otherwise we will skip the first header short
				stream.seekg(sizeof(uint16), std::istream::beg);
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
