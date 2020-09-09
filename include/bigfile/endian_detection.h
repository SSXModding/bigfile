#pragma once
#include <bigfile/types.h>

namespace bigfile {

	/**
	 * Endian enumeration
	 */
	enum class endian : byte {
		little,
		big,
	};

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ || defined(_WIN32)
	constexpr endian current_endian = endian::little;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	constexpr endian current_endian = endian::big;
#endif

} // namespace bigfile