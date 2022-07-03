//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

#include "ReadFileOp.h"

#include <iostream>

namespace bigfile::detail {

	bool ReadFileOp::operator()() {
		// While this is partially a footgun,
		// the constructor verifies that this should
		// work.
		//
		// We also assert that the input stream
		// should stay around until at least the archive
		// is removed.
		// So it's sleazy, but it's sleazy that should work.

		auto& is = (*archive.inputStream).get();

		auto last_offset = is.tellg();

		is.seekg(file.offset, std::istream::beg);

		switch(archive.packType) {
			case PackType::Uncompressed:
				file.data.resize(file.size);
				if(!is.read(reinterpret_cast<char*>(&file.data[0]), file.size))
					return false;
				break;

			case PackType::RefPack: {
				// Read compressed data into intermediate buffer
				std::vector<std::uint8_t> intBuffer;
				intBuffer.resize(file.compressed_size);

				if(!is.read(reinterpret_cast<char*>(&intBuffer[0]), file.compressed_size))
					return false;

				// Do the magic
				auto decompressed = refpack::Decompress(intBuffer);
				if(decompressed.empty())
					return false;

				// resize final buffer and copy the decompressed data to it
				file.data.resize(file.size);
				memcpy(&file.data[0], &decompressed[0], file.size);
			} break;

			default:
				//?????
				return false;
		}

		is.seekg(last_offset, std::istream::beg);
		return true;
	}

} // namespace bigfile::detail