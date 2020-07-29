// bigfile: a C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.


// CO-FB format data structures (and operator defintions)

#include <cstdint>
#include <vector>
#include <string>

namespace bigfile {

	// we don't one-shot read any COFB data (except for files) in,
	// cause we simply can't due to how it's designed.

	// BIG header
	struct CoFbHeader {
		threebyte_t strtable_size;
		threebyte_t file_count;
	};

	struct CoFbFileHeader {
		threebyte_t offset;
		threebyte_t length;
	};

	struct CoFbFile : public CoFbFileHeader {
		std::vector<char> data;

		static CoFbFile Empty;
	};

}
