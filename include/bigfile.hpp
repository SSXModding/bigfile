// bigfile: a C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// Main bigfile header.

// core type defintions
namespace bigfile {

	// this is actually four bytes, but
	// used as a marker for stuff that needs to be three bytes
	typedef std::uint32_t threebyte_t;

}

#include <bigfile/utility_functions.hpp>
#include <bigfile/structures/bigf.hpp>
#include <bigfile/structures/cofb.hpp>
#include <bigfile/big_archive.hpp>
