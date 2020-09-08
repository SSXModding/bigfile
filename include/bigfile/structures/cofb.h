#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <bigfile/types.h>

namespace bigfile {

	// we don't one-shot read any COFB data (except for files) in,
	// cause we simply can't due to how the structures are laid out.
	// Thanks, EA!

	
	// File type for COFB files.
	enum class CofbFileType {
		Uncompressed,
		// COFB files are usually packed in Refpack format.
		// In some cases (see SSXFE.BIG which is a COFB archive's tricky.ser) this is not true.
		Refpack
	};

	// COFB header
	struct CoFbHeader {
		uint16 index_table_size;
		uint16 file_count;
	};

	struct CoFbFileHeader {
		threebyte_t offset;
		threebyte_t compressed_length;
	};
}
