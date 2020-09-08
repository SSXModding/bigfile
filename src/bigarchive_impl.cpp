#pragma once

#include <optional>

#include <bigfile/big_archive.h>
#include <bigfile/utility_functions.h>

#include <bigfile/refpack.h>

namespace bigfile {

	/**
	 * endian enumeration
	 */
	enum class endian : byte {
		little,
		big
	};

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ || defined(_WIN32)
	constexpr endian current_endian = endian::little;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	constexpr endian current_endian = endian::big;
#endif

	/**
	 * Swap a big endian thing if the machine is little endian.
	 * Generates no code if not needed!
	 */
	template<typename T>
	constexpr void SwapBE(T& thing) {
		if constexpr (current_endian == endian::little)
			thing = SwapEndian(thing);
	}

	/**
	 * Swap a little endian thing if the machine is big endian.
	 * Generates no code if not needed!
	 */
	template<typename T>
	constexpr void SwapLE(T& thing) {
		if constexpr (current_endian == endian::big)
			thing = SwapEndian(thing);
	}


	/** 
	 * Read a uint24 value.
	 */
	threebyte_t ReadUint24(std::istream& stream) {
		byte buf[3];
		stream.read((char*)&buf, 3);

		return ((buf[0] << 16) | (buf[1] << 8) | buf[2]);
	}

	
	// TODO: this code will be a bit bloat depending on
	// the archive size, 
	// however this is pratically the only way to do it for a while
	//
	// Investigate a possible way of (re) implementing the code that
	// would cache the TOC and read data in when user requested a file...

	struct BigArchive::Impl {
		/**
		 * Read a BIGF archive.
		 */
		void ReadBigfImpl(std::istream& stream) {
			BigHeader header;

			type = ArchiveType::BIGF;

			ReadStruct(stream, header);

			SwapBE(header.filecount);
			SwapBE(header.index_table_size);

			// read files
			for(std::uint32_t i = 0; i < header.filecount; ++i) {
				std::string filename;
				BigFileHeader fheader;
				File file;
				ReadStruct(stream, fheader);

				SwapBE(fheader.offset);
				SwapBE(fheader.length);

				// get the filename of the file we're going to read
				while(true) {
					char c = stream.get();
					if(c == '\0')
						break;

					filename += c;
				}

				auto oldpos = stream.tellg();

				stream.seekg(fheader.offset, std::istream::beg);

				file.data.resize(fheader.length);
				stream.read((char*)file.data.data(), fheader.length);

				stream.seekg(oldpos, std::istream::beg);

				files.insert({ filename, file });
			}
		}

		/**
		 * Read a CoFB archive. 
		 */
		void ReadCofbImpl(std::istream& stream) {
			CoFbHeader header;

			type = ArchiveType::CoFb;

			ReadStruct(stream, header);
			
			SwapBE(header.file_count);
			SwapBE(header.index_table_size);

			for(std::uint32_t i = 0; i < header.file_count; ++i) {
				std::string filename;
				CoFbFileHeader fheader;
				File file;
				
				fheader.offset = ReadUint24(stream);
				fheader.compressed_length = ReadUint24(stream);
				
				SwapLE(fheader.offset);
				SwapLE(fheader.compressed_length);

				// get the filename of the file we're going to read
				while(true) {
					char c = stream.get();
					if(c == '\0')
						break;

					filename += c;
				}

				auto oldpos = stream.tellg();
				std::vector<byte> decompressed(fheader.compressed_length);
				stream.seekg(fheader.offset, std::istream::beg);

				stream.read((char*)decompressed.data(), fheader.compressed_length);

				if(decompressed[0] == 0x10 && decompressed[1] == 0xFB) {
					// Refpack compressed file
					file.type = CofbFileType::Refpack;
					file.data = refpack::Decompress(MakeSpan(decompressed.data(), fheader.compressed_length));
				} else {
					file.data = decompressed;
				}

				stream.seekg(oldpos, std::istream::beg);

				files.insert({ filename, file });

			}
		
		}

		std::optional<File> GetFileImpl(const std::string& filename) {
			try {
				return files.at(filename);
			} catch(std::out_of_range) {
				// return an empty optional, to avoid bloating
				// no-exceptions (unless required) rule
				return {};
			}
		}

		// Impl variables that can be accessed

		std::map<std::string, File> files;
		ArchiveType type;
	};

} // namespace bigfile

