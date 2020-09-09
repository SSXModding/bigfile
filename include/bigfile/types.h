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

	/**
	 * 4-byte "three-byte" type.
	 */
	typedef uint32 threebyte_t;

	/**
	 * Size of a threebyte_t.
	 */
	constexpr std::size_t threebyte_size = 3u;

} // namespace bigfile
