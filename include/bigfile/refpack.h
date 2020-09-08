#pragma once

#include <vector>
#include <bigfile/types.h>
#include <bigfile/span.h>

namespace bigfile {
	namespace refpack {

		/**
		 * Decompress a RefPack source.
		 * 
		 * \param[in] compressed Compressed span of data.
		 * \return A decompressed buffer.
		 */
		std::vector<byte> Decompress(Span<byte> compressed);

	} // namespace refpack
} // namespace bigfile