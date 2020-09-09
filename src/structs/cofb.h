#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <bigfile/types.h>

namespace bigfile {

	// we don't one-shot read any C0FB data (except for files & the file header) in,
	// cause we simply can't due to how the structures are laid out.
	// Thanks, EA!

	// C0FB header
	struct CoFbHeader {
		uint16 IndexTableSize;
		uint16 FileCount;
	};

	/**
	 * Index header
	 */
	struct CoFbFileHeader {
		threebyte_t Offset;
		threebyte_t CompressedLength;
	};
}
