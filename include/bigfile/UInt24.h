//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

#ifndef BIGFILE_UINT24_H
#define BIGFILE_UINT24_H

#include <cstdint>

namespace bigfile {

	namespace detail {

		/**
		 * A sane 24-bit integer type.
		 */
		template <bool BE>
		struct basic_uint24 {
		   private:
			constexpr void Set(std::uint32_t val) {
				if constexpr(BE) {
					bytes_[0] = (val >> 16) & 0xFF;
					bytes_[1] = (val >> 8) & 0xFF;
					bytes_[2] = val & 0xFF;
				} else {
					bytes_[2] = (val >> 16) & 0xFF;
					bytes_[1] = (val >> 8) & 0xFF;
					bytes_[0] = val & 0xFF;
				}
			}

			constexpr std::uint32_t Get() {
				if constexpr(BE) {
					return bytes_[0] << 16 | bytes_[1] << 8 | bytes_[2];
				} else {
					return bytes_[2] << 16 | bytes_[1] << 8 | bytes_[0];
				}
			}

		   public:
			constexpr operator std::uint32_t() { // NOLINT (We want this to be implicitly convertiable in expressions)
				return Get();
			}

			constexpr basic_uint24& operator=(std::uint32_t val) {
				Set(val);
				return *this;
			}

			constexpr basic_uint24& operator=(basic_uint24 val) {
				Set(val.Get());
				return *this;
			}

			constexpr basic_uint24& operator*(const basic_uint24& other) {
				Set(Get() * other.Get());
			}

			constexpr basic_uint24& operator+(const basic_uint24& other) {
				_Set(Get() + other.Get());
			}

			constexpr basic_uint24& operator-(const basic_uint24& other) {
				_Set(Get() - other.Get());
			}

			constexpr basic_uint24& operator/(const basic_uint24& other) {
				_Set(Get() / other.Get());
			}

			constexpr basic_uint24& operator^(const basic_uint24& other) {
				_Set(Get() ^ other.Get());
			}

			// NO TOUCHY
			std::uint8_t bytes_[3];
		};

		// sanity checks
		static_assert(sizeof(basic_uint24<false>) == 3, "Your compiler sucks and basic_uint24<> somehow isn't 3 bytes on it. Get a better one");
	} // namespace detail

	// uint24 types

	/**
	 * Little endian unsigned 24-bit integer.
	 */
	using uint24_le_t = detail::basic_uint24<false>;

	/**
	 * Big endian unsigned 24-bit integer.
	 */
	using uint24_be_t = detail::basic_uint24<true>;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ || defined(_WIN32)
	using uint24_t = detail::basic_uint24<false>;
#else
	using uint24_t = detail::basic_uint24<true>;
#endif

} // namespace bigfile

#endif // BIGFILE_UINT24_H
