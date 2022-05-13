//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

#ifndef BIGFILE_REFPACK_H
#define BIGFILE_REFPACK_H

#include <cstdint>
#include <vector>

namespace bigfile::refpack {

	/**
	 * Decompress a RefPack compressed source.
	 *
	 * \param[in] compressed Compressed data.
	 * \return A decompressed buffer, or an empty buffer on error..
	 */
	std::vector<std::uint8_t> Decompress(const std::vector<std::uint8_t>& compressed);

} // namespace bigfile::refpack

#endif