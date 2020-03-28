// bigfile: a header-only C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// Implementation of BigArchive.

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ || defined(_WIN32)
	#define BIGFILE_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	#define BIGFILE_BIG_ENDIAN
#endif

#define SWAP_MEMBER(o, m) o.m = EndianSwap(o.m)

namespace detail {

	BigArchive::~BigArchive() {
		if(reading) {
			reading = false;
			read_stream.reset();
		}

		files.clear();
	}


	bool BigArchive::ReadFrom(std::shared_ptr<std::istream> stream) {
		if(!stream)
			return false;

		reading = true;
		read_stream = std::shared_ptr<std::istream>(stream);

		files.clear();

		// read in the header
		read_stream->read((char*)&header, sizeof(BigHeader));

		if(!MagicCompare(header.magic))
			return false;

		// swap things from big endian to little endian
		// if applicable for the platform
#ifdef BIGFILE_LITTLE_ENDIAN
		SWAP_MEMBER(header, file_count);
		SWAP_MEMBER(header, first_offset);
#endif

		// read in file table
		for(std::uint32_t i = 0; i < header.file_count; ++i) {
			std::string filename;
			BigFile file;
			read_stream->read((char*)&file, sizeof(BigFileHeader));

#ifdef BIGFILE_LITTLE_ENDIAN
			SWAP_MEMBER(file, offset);
			SWAP_MEMBER(file, length);
#endif
			// *back crunching noises*
			while(true) {
				char c = read_stream->get();
				if(c == '\0')
					break;
	
				filename += c;
			}

			files[filename] = file;
		}
			
		return true;
	}

	BigFile& BigArchive::GetFile(const std::string& path) {
		if(!reading)
			return BigFile::Empty;

		auto it = files.find(path);
		if(it == files.end())
			return BigFile::Empty; // not in archive

		BigFile& file = it->second;

		if(file.data.empty()) {
			// read in file
			file.data.resize(file.length);

			auto old_pos = read_stream->tellg();
			read_stream->seekg(file.offset, std::istream::beg);
			read_stream->read((char*)&file.data[0], file.length);

			read_stream->seekg(old_pos, std::istream::beg);
		}

		return file;			
	}

	std::shared_ptr<BigArchive> BigArchive::GetNestedArchive(const std::string& path) {
		auto file = GetFile(path);

		if(file.length == 0)
			return nullptr;

		char magic_check[4];			
		std::shared_ptr<std::istream> stream (new std::istringstream {file.data.data()});
			
		stream->read(magic_check, 4);
		if(!MagicCompare(magic_check)) {
			stream->clear(); // clean up stream
			stream.reset();
			return nullptr;
		}

		stream->seekg(0, std::istringstream::beg);
		return std::make_shared<BigArchive>(stream);
	}

	std::vector<std::string> BigArchive::GetPaths() {
		std::vector<std::string> temp;

		for(auto&& t : files)
			temp.push_back(t.first);

		return temp;
	}
}

#ifdef BIGFILE_LITTLE_ENDIAN
	#undef BIGFILE_LITTLE_ENDIAN
#endif

#ifdef BIGFILE_BIG_ENDIAN
	#undef BIGFILE_BIG_ENDIAN
#endif

#undef SWAP_MEMBER
