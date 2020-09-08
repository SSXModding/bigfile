#pragma once

#include <cstdint>

// Core type defintitions

namespace bigfile {

	typedef std::uint8_t byte;
	typedef std::int8_t sbyte;

	typedef std::int16_t int16;
	typedef std::uint16_t uint16;

	typedef std::int32_t int32;
	typedef std::uint32_t uint32;

	typedef std::int64_t int64;
	typedef std::uint64_t uint64;

	// this is actually four bytes, but
	// used as a marker for stuff that needs to be three bytes and to the reader
	typedef std::uint32_t threebyte_t;

} // namespace bigfile
