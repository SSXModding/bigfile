#include <bigfile/BigArchive.h>
#include <bigfile/RefPack.h>
#include <bigfile/structs/BigStructs.h>
#include <bigfile/UtilityFunctions.h>

#include "ops/ReadFileOp.h"
#include "ops/ReadHeaderAndTocOp.h"

namespace bigfile {

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
			constexpr static char BigfMagic[] = "BIGF";

			stream.read((char*)&magic, 4);

			if(!stream)
				return ArchiveType::NotArchive;

			stream.seekg(0, std::istream::beg);

			if(!strncmp(reinterpret_cast<char*>(&magic[0]), BigfMagic, sizeof(BigfMagic) - 1))
				return ArchiveType::BIGF;

			if(magic[0] == 0xC0 && magic[1] == 0xFB)
				return ArchiveType::C0FB;

			// Neither magic matched, so it's probably not supported.
			return ArchiveType::NotArchive;
		}();

		// Read header and TOC for given archive type
		switch(format) {
			case ArchiveType::BIGF:
				return detail::ReadHeaderAndTocOp<BigFileHeader, BigTocHeader> { stream }(files);

			case ArchiveType::C0FB:
				return detail::ReadHeaderAndTocOp<CoFbFileHeader, CoFbTocHeader> { stream }(files);

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
					if(!detail::ReadFileOp { *inputStream, file }())
						// .. or if that fails, give up, even if it's in the map,
						// since the user requested data and isn't getting it.
						return {};
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
