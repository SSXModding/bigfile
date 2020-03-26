#include <cstdint>
#include <vector>
#include <string>

namespace detail {

	// BIG header
	struct BigHeader {
		char magic[4];

		// Little endian
		uint32_t archive_size;

		uint32_t file_count;
		uint32_t first_offset;
	};

	struct BigFileHeader {
		uint32_t offset;
		uint32_t length;
	};

	struct BigFile : public BigFileHeader {
		std::vector<char> data;
	};

}
