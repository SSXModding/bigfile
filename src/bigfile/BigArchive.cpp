#include <bigfile/BigArchive.h>
#include <bigfile/RefPack.h>
#include <bigfile/structs/BigStructs.h>
#include <bigfile/UtilityFunctions.h>

#include "ops/ReadFileOp.h"
#include "ops/ReadHeaderAndTocOp.h"

namespace bigfile {

	BigArchive::File::File(BigArchive& archive)
		: parentArchive(archive) {

	}

	bool BigArchive::File::IsInMemory() const {
		return !data.empty();
	}

	void BigArchive::File::Done() {
		data.clear();
	}

	bool BigArchive::File::ReadFile() {
		if(!IsInMemory())
			return detail::ReadFileOp { *parentArchive.inputStream, parentArchive, *this }();

		// Fast path - return true.
		return true;
	}

	std::uint32_t BigArchive::File::GetOffset() const {
		return offset;
	}

	std::uint32_t BigArchive::File::GetSize() const {
		return size;
	}

	std::uint32_t BigArchive::File::GetCompressedSize() const {
		// assert(compressed_size != 0 && "Not compressed. API contract says no"); ?
		return compressed_size;
	}

	const std::vector<std::uint8_t>& BigArchive::File::GetData() const {
		return data;
	}

 	// BigArchive itself

	BigArchive::BigArchive(ArchiveType type, PackType packType) {
		InitArchive(type, packType);
	}

	bool BigArchive::ReadArchive(std::istream& stream) {
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

		type = format;

		// Read header and TOC for given archive type
		switch(format) {
			case ArchiveType::BIGF:
				return detail::ReadHeaderAndTocOp<BigFileHeader, BigTocHeader> { stream, *this }();

			case ArchiveType::C0FB:
				return detail::ReadHeaderAndTocOp<CoFbFileHeader, CoFbTocHeader> { stream, *this }();

			case ArchiveType::NotArchive:
			default:
				return false;
		}
	}

	void BigArchive::InitArchive(ArchiveType type, PackType packType) {
		this->type = type;
		this->packType = packType;
	}

	ArchiveType BigArchive::GetArchiveType() const {
		return type;
	}

	PackType BigArchive::GetPackType() const {
		return packType;
	}

	std::optional<LumpyDebugInfo> BigArchive::GetDebugInfo() const {
		return debugInfo;
	}

	BigArchive::iterator BigArchive::begin() {
		return files.begin();
	}

	BigArchive::iterator BigArchive::end() {
			return files.end();
	}

	BigArchive::const_iterator BigArchive::cbegin() {
		return files.cbegin();
	}

	BigArchive::const_iterator BigArchive::cend() {
		return files.cend();
	}

	BigArchive::iterator BigArchive::insert(const std::string& path, const std::vector<std::uint8_t>& data) {
		// create a new file with data from the user
		File f(*this);
		f.data = data;
		f.size = data.size();

		auto [it, success] = files.insert({path, f});
		if(success)
			return it;

		// maybe throw? idk
		return files.end();
	}

	BigArchive::File& BigArchive::operator[](const std::string& path) {
		return files.at(path);
	}

	const BigArchive::File& BigArchive::operator[](const std::string& path) const {
		return files.at(path);
	}

} // namespace bigfile
