#include <iostream>
#include <algorithm>
#include <cstring>

namespace detail {

	// god is dead
	// upside is, on platforms where bswap can become a single instruction,
	// it will become one (usually).
	template<typename T>
	inline T EndianSwap(T value) {
		if constexpr(sizeof(T) == 2) {
			return __builtin_bswap16((uint16_t)value);
		} else if constexpr(sizeof(T) == 4) {
			return __builtin_bswap32((uint32_t)value);
		} else if constexpr(sizeof(T) == 8) {
			return __builtin_bswap64((uint64_t)value);
		}
	}

	inline bool MagicCompare(char value[4]) {
		constexpr static char magic[] = "BIGF";

		if(!strncmp(value, magic, 4))
			return true;

		return false;
	}

}
