#include <bigfile/big_archive.h>
#include <fstream>

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

	// writing fun

	bool BigArchive::SetupArchive(ArchiveType type) {
		impl->files.clear();
		impl->type = type;
		return true;
	}

	bool BigArchive::AddFile(const std::string& path, File& file, CofbFileType type) {
		auto it = impl->files.find(path);

		// cannot have multiple files
		if(it != impl->files.end())
			return false;
	}


	bool BigArchive::ReplaceFile(const std::string& path, File& file) {
		auto it = impl->files.find(path);

		// ReplaceFile needs a pre-existing file
		if(it == impl->files.end())
			return false;
		
		// TODO: ReplaceFile() should be able
		// to handle "intelligent" file swaps by figuring out if its a file in the archive
		// if so we can swap the two around and bam

		auto& arfile = (it->second);

		// overwrite all data with the given file data
		// excluding the file type. We ignore that.
		arfile.compressed_size = file.compressed_size;
		arfile.size = file.size;
		arfile.data = file.data;

		// well, we did it?
		return true;
	}

	bool BigArchive::RemoveFile(const std::string& path) {
		auto it = impl->files.find(path);

		// that file didn't exist in the first place...
		if(it == impl->files.end())
			return false;

		impl->files.erase(it);
		return true;
	}

	bool WriteArchive(std::ofstream& stream) {
		if(!stream)
			return false;

		
	}

} // namespace bigfile
