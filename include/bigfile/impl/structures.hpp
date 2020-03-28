// bigfile: a header-only C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// Implementation of operators for BigFile
// and instantation of the special Empty object.

namespace detail {

	BigFile BigFile::Empty = { (uint32_t)-1, (uint32_t)-1 };

	bool operator!(BigFile& file) {
	#define cmp(v) (file.v != BigFile::Empty.v)
		return (&file != &BigFile::Empty) 
				&& cmp(offset) && cmp(length) && cmp(data);
	#undef cmp
	}

}
