// bigfile: a C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.


// BIG format data structures (and operator defintions)

#include <cstdint>
#include <vector>
#include <string>

namespace bigfile {

	// BIG header
	struct BigHeader {
		uint32_t archive_size;

		uint32_t filecount;
		// not sure
		uint32_t firstoffset;
	};


	struct BigFileHeader {
		uint32_t offset;
		uint32_t length;
	};

	struct BigFile : public BigFileHeader {
		std::vector<char> data;

		static BigFile Empty;
	};


	bool operator!(BigFile& file);

}
