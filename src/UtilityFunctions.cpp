// bigfile: a header-only C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// Implemntation of utlity functions used in bigfile.

#include <bigfile/ArchiveType.h>

#include <cstring>

namespace bigfile {

	ArchiveType GetArchiveType(std::uint8_t* value) {
		constexpr static char BigfMagic[] = "BIGF";

		if(!value)
			return ArchiveType::NotArchive;

		if(!strncmp((char*)value, BigfMagic, sizeof(BigfMagic) - 1))
			return ArchiveType::BIGF;

		if(value[0] == 0xC0 && value[1] == 0xFB)
			return ArchiveType::C0FB;

		// Neither magic matched, so it's probably not supported.
		return ArchiveType::NotArchive;
	}

} // namespace bigfile
