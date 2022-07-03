//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

#ifndef BIGFILE_UTILITYFUNCTIONS_H
#define BIGFILE_UTILITYFUNCTIONS_H

#include <bigfile/ArchiveType.h>

#include <algorithm>
#include <bit>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#include <intrin.h>
	// Prefer the intrinsic function versions
	// of the following functions, if they
	// exist on the target.
	#pragma intrinsic(_byteswap_ushort)
	#pragma intrinsic(_byteswap_ulong)
	#pragma intrinsic(_byteswap_uint64)

	// Byteswap a 16-bit field.
	#define BYTESWAP16(x) _byteswap_ushort(x)

	// Byteswap a 32-bit field.
	#define BYTESWAP32(x) _byteswap_ulong(x)

	// Byteswap a 64-bit field.
	#define BYTESWAP64(x) _byteswap_uint64(x)
#elif defined(__GNUC__)
		// Builtin functions with GNU C get turned into (sometimes single-instruction) intrinsics
		// usually by default if the target supports them. Otherwise,
		// they become inline functions (which still *have* a speed penalty,
		// but far less then if it had to make a call into the C runtime)

		// Byteswap a 16-bit field.
	#define BYTESWAP16(x) __builtin_bswap16(x)

		// Byteswap a 32-bit field.
	#define BYTESWAP32(x) __builtin_bswap32(x)

		// Byteswap a 64-bit field.
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
	template <typename T>
	constexpr T SwapEndian(T value) {
		if constexpr(sizeof(T) == 2) {
			return BYTESWAP16(value);
		} else if constexpr(sizeof(T) == 4) {
			return BYTESWAP32(value);
		} else if constexpr(sizeof(T) == 8) {
			return BYTESWAP64(value);
		} else {
			// slower than an individual N-byte field, but
			// whatever. At least we can swap the endian of anything we want this way

			static_assert(sizeof(T) % sizeof(std::uint16_t) != 0, "T must be evenly divisible by sizeof(std::uint16_t).");

			T temp {value};
#if 0
			// copy value-structure to temp structure we will return
			//memcpy(&temp, &value, sizeof(T));
#endif

			// This works by treating the temporary structure
			// as if it was an array of 16bit fields.
			for(std::size_t i = 0; i < sizeof(T); i += sizeof(std::uint16_t))
				reinterpret_cast<std::uint16_t*>(&temp)[i] = BYTESWAP16(reinterpret_cast<std::uint16_t*>(&temp)[i]);

			return temp;
		}
	}

// undefine internal macros.
#undef BYTESWAP16
#undef BYTESWAP32
#undef BYTESWAP64

	template <typename T>
	constexpr void SwapBE(T& thing) {
		if constexpr(std::endian::native == std::endian::little) // NOLINT
			thing = SwapEndian(thing);
	}

	template <typename T>
	constexpr void SwapLE(T& thing) {
		if constexpr(std::endian::native == std::endian::big) // NOLINT
			thing = SwapEndian(thing);
	}

} // namespace bigfile

#endif // BIGFILE_UTILITYFUNCTIONS_H