#include <bigfile/BigArchive.h>
#include <bigfile/RefPack.h>
#include <bigfile/structs/BigStructs.h>
#include <bigfile/UtilityFunctions.h>

#include <fstream>
#include <iostream>


namespace bigfile {

	/**
	 * Generic class to read the header and TOC.
	 * Could technically be made lazy.
	 */
	template <class TFileHeader, class TTocHeader>
	struct ReadHeaderAndTocOp {

		/**
		 * Seek rewinder.
		 * Go play Unturned if you don't get the joke.
		 */
		struct SeekyWheelyAutomobiley {
			explicit SeekyWheelyAutomobiley(ReadHeaderAndTocOp& rh)
				: rh(rh) {
				last_offset = this->rh.is.tellg();
			}

			~SeekyWheelyAutomobiley() {
				rh.is.seekg(last_offset, std::istream::beg);
			}

		   private:
			ReadHeaderAndTocOp& rh;
			std::size_t last_offset;
		};

		explicit ReadHeaderAndTocOp(std::istream& is)
			: is(is) {
		}

		bool operator()(std::map<std::string, BigArchive::File>& files) {
			TFileHeader header {};
			header.Read(is);

			if(!is)
				return false;

			for(auto i = 0; i < header.FileCount; ++i) {
				TTocHeader fileTocEntry {};
				BigArchive::File file;
				fileTocEntry.Read(is);

				file.offset = fileTocEntry.Offset;

				if(!is)
					break;

				// Check if this file is RefPack packed.
				// If so, set the packtype appropriately.
				{
					SeekyWheelyAutomobiley rewinder(*this);
					std::uint8_t refpack_test[sizeof(std::uint16_t)];

					is.seekg(fileTocEntry.Offset, std::istream::beg);
					is.read(reinterpret_cast<char*>(&refpack_test[0]), sizeof(std::uint16_t));

					if(refpack_test[0] == 0x10 && refpack_test[1] == 0xFB) {
						file.type = BigArchive::File::PackType::Refpack;
						file.compressed_size = fileTocEntry.Length;
					} else {
						file.size = fileTocEntry.Length;
					}
				}

				files[fileTocEntry.Filename] = file;
			}

			return true;
		}

	   private:
		std::istream& is;
	};

	/**
	 * Lazy file reader operation.
	 */
	struct ReadFileOp {
		explicit ReadFileOp(std::istream& is, BigArchive::File& file)
			: is(is), file(file) {
		}

		bool operator()() {
			auto last_offset = is.tellg();

			is.seekg(file.offset, std::istream::beg);

			switch(file.type) {
				case BigArchive::File::PackType::Uncompressed:
					file.data.resize(file.size);
					if(!is.read(reinterpret_cast<char*>(&file.data[0]), file.size))
						return false;
					break;

				case BigArchive::File::PackType::Refpack: {
					std::vector<std::uint8_t> intBuffer;
					intBuffer.resize(file.compressed_size);

					if(!is.read(reinterpret_cast<char*>(&intBuffer[0]), file.compressed_size))
						return false;

					auto decompressed = refpack::Decompress(intBuffer);
					if(decompressed.empty())
						return false;

					file.size = decompressed.size();
					file.data.resize(file.size);
					memcpy(&file.data[0], &decompressed[0], file.size);
				} break;

				default:
					//?????
					return false;
			}

			is.seekg(last_offset, std::istream::beg);
			return true;
		}

	   private:
		std::istream& is;
		BigArchive::File& file;
	};

	bool BigArchive::File::IsInMemory() const {
		return !data.empty();
	}

	void BigArchive::File::Done() {
		data.clear();
	}

	bool BigArchive::ReadToc(std::istream& stream) {
		if(!stream)
			return false;

		inputStream = stream;

		auto format = [&]() {
			// Do a magic check
			std::uint8_t magic[4];

			stream.read((char*)&magic, 4);

			if(!stream)
				return ArchiveType::NotArchive;

			stream.seekg(0, std::istream::beg);

			return bigfile::GetArchiveType(magic);
		}();

		switch(format) {
			case ArchiveType::BIGF:
				return ReadHeaderAndTocOp<BigFileHeader, BigTocHeader> { stream }(files);

			case ArchiveType::C0FB:
				return ReadHeaderAndTocOp<CoFbFileHeader, CoFbTocHeader> { stream }(files);

			case ArchiveType::NotArchive:
			default:
				return false;
		}
	}

	ArchiveType BigArchive::GetArchiveType() {
		return type;
	}

	std::optional<std::reference_wrapper<BigArchive::File>> BigArchive::GetFile(const std::string& path, bool wantsData) {
		try {
			auto& file = files.at(path);

			if(wantsData) {
				// If the file's not in memory, and we want it,
				// try to read it.
				if(!file.IsInMemory())
					if(!ReadFileOp { *inputStream, file }())
						return {}; // .. or if that fails, give up, even if it's in the map.
			}

			return file;
		} catch(std::out_of_range& oor) {
			// File isn't in the map, so it's probably an invalid path.
			return {};
		}
	}

	std::vector<std::string> BigArchive::GetPaths() {
		std::vector<std::string> temp;

		for(auto&& t : files)
			temp.push_back(t.first);

		return temp;
	}

} // namespace bigfile
