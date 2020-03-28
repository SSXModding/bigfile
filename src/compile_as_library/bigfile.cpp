// bigfile: a header-only C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// Sample .cpp file to compile all of bigfile as one translation unit.
// To use bigfile like this,
// define BIGFILE_NO_IMPL before including the bigfile header.

#define BIGFILE_NO_IMPL
#include <bigfile.hpp>

#include <bigfile/impl/big_archive.hpp>
#include <bigfile/impl/structures.hpp>
#include <bigfile/impl/utility_functions.hpp>
