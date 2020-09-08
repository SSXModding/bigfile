#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace bigfile {

	// BIG header
	struct BigHeader {
		uint32_t archive_size;

		uint32_t filecount;

		uint32_t index_table_size;
	};


	struct BigFileHeader {
		uint32_t offset;
		uint32_t length;
	};

}
