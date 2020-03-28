// bigfile: a header-only C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// Main bigfile header.

#include "bigfile/utility_functions.hpp"
#include "bigfile/structures.hpp"
#include "bigfile/big_archive.hpp"

#if !defined(BIGFILE_NO_IMPL)
	#include "bigfile/impl/big_archive.hpp"
	#include "bigfile/impl/structures.hpp"
	#include "bigfile/impl/utility_functions.hpp"
#endif

// Exports of bigfile.
namespace bigfile {
	using detail::BigFileHeader;
	using detail::BigFile;
	using detail::BigArchive;
}
