// bigfile: a header-only C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// Definition (and slight implementation) of utlity functions used in bigfile.

#include <iostream>
#include <algorithm>
#include <cstring>

#ifdef MSC_VER
	#include <intrin.h>
	// Prefer the intrinsic function versions
	// of the following functions, if they
	// exist on the target.
	#pragma intrinsic(_byteswap_ushort)
	#pragma intrinsic(_byteswap_ulong)
	#pragma intrinsic(_byteswap_uint64)

	#define BYTESWAP16(x) _byteswap_ushort(x)
	#define BYTESWAP32(x) _byteswap_ulong(x)
	#define BYTESWAP64(x) _byteswap_uint64(x)
#elif defined(__GNUC__)
	// Builtin functions with GNU C get turned into (sometimes single-instruction) intrinisics
	// usually by default if the target supports them. Otherwise,
	// they become inline functions (which still *have* a speed penalty, 
	// but far less then if it had to make a call into the C runtime)
	#define BYTESWAP16(x) __builtin_bswap16(x)
	#define BYTESWAP32(x) __builtin_bswap32(x)
	#define BYTESWAP64(x) __builtin_bswap64(x)
#else
#error Unsupported compiler.
#endif


namespace detail {

	template<typename T>
	inline T EndianSwap(T value) {
		if constexpr(sizeof(T) == 2) {
			return BYTESWAP16(value);
		} else if constexpr(sizeof(T) == 4) {
			return BYTESWAP32(value);
		} else if constexpr(sizeof(T) == 8) {
			return BYTESWAP64(value);
		}
	}

	bool MagicCompare(char value[4]);

}

#undef BYTESWAP16
#undef BYTESWAP32
#undef BYTESWAP64

