#pragma once

#include <bigfile/types.h>

namespace bigfile {

	/**
	 * BIGF file header
	 */
	struct BigHeader {
		uint32 ArchiveSize;

		uint32 FileCount;

		uint32_t IndexTableSize;
	};

	
	/**
	 * BIGF per-file header
	 */
	struct BigFileHeader {
		uint32 Offset;
		uint32 Length;
	};

}
