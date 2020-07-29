// bigfile: a C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// Definition (and implementation) of utlity functions used in bigfile.

#include <iostream>
#include <algorithm>
#include <cstring>
#include <bigfile/archive_type.hpp>

#ifdef _MSC_VER
	#include <intrin.h>
	// Prefer the intrinsic function versions
	// of the following functions, if they
	// exist on the target.
	#pragma intrinsic(_byteswap_ushort)
	#pragma intrinsic(_byteswap_ulong)
	#pragma intrinsic(_byteswap_uint64)

	// Define our portable byteswap macros for MSVC.
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


namespace bigfile {

	/**
	 * Swap the endian of a provided value.
	 *
	 * \tparam T Type
	 * \param[in] value value to swap endian of
	 */
	template<typename T>
	inline T swap_endian(T value) {
		if constexpr(sizeof(T) == 2) {
			return BYTESWAP16(value);
		} else if constexpr(sizeof(T) == 4) {
			return BYTESWAP32(value);
		} else if constexpr(sizeof(T) == 8) {
			return BYTESWAP64(value);
		} else {
			// swap sizeof(std::uint16_t) bytes at a time of any structure.
			// slower than an individual N-bit field, but
			// whatever. At least we can swap the endian of anything we want.

			T temp{};
			
			// copy value-structure to temp structure
			memcpy(&temp, &value, sizeof(T));

			// perform swap
			for(int i = 0; i < sizeof(T); i += sizeof(std::uint16_t))
				((std::uint16_t*)&temp)[i] = BYTESWAP16(((std::uint16_t*)&temp)[i]);

			return temp;
		}
	}

	ArchiveType GetArchiveType(char* value);

}

// undefine internal macros.
#undef BYTESWAP16
#undef BYTESWAP32
#undef BYTESWAP64

