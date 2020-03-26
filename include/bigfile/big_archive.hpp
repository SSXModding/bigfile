#include <iostream>
#include <vector>
#include <map>
#include <string>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ || defined(_WIN32)
	#define BIGFILE_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	#define BIGFILE_BIG_ENDIAN
#endif


#define SWAP_MEMBER(o, m) o.m = EndianSwap(o.m)

namespace detail {

	struct BigArchive {

		// default constructor.
		BigArchive() {
			
		}

		// constructor to auto read from stream
		BigArchive(std::istream& stream) {
			ReadFrom(stream);
		}

		~BigArchive() {
			
		}

		bool ReadFrom(std::istream& stream) {
			reading = true;
			read_stream = &stream;

			// TODO(modeco80): clear out file table
			//				   when reading in a new archive

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


		BigFile GetFile(const std::string& path) {
			if(!reading)
				return {};

			auto it = files.find(path);

			if(it == files.end())
				return {}; // not in archive

			BigFile file = it->second;

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
		
		std::vector<std::string> GetAllPaths() {
			std::vector<std::string> temp;

			for(auto t : files)
				temp.push_back(t.first);

			return temp;
		}

		std::map<std::string, BigFile> files;

	private:
		BigHeader header;
		
		std::istream* read_stream;
		bool reading = false;
	};
	
#ifdef BIGFILE_LITTLE_ENDIAN
	#undef BIGFILE_LITTLE_ENDIAN
#endif

#ifdef BIGFILE_BIG_ENDIAN
	#undef BIGFILE_BIG_ENDIAN
#endif

#undef SWAP_MEMBER
}
