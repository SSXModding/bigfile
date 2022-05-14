//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

#ifndef BIGFILE_ARCHIVETYPE_H
#define BIGFILE_ARCHIVETYPE_H

#include <cstdint>

namespace bigfile {

	/**
	 * BIG archive type.
	 */
	enum class ArchiveType : std::uint8_t {
		NotArchive,

		BIGF, ///< Old style 32-bit.

		C0FB ///< Old style 24-bit.
	};

	enum PackType : std::uint8_t {
		Uncompressed, ///< Uncompressed.

		RefPack ///< EA RefPack (LZSS-like).

	};

}

#endif