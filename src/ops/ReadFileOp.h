//
// SSX 3 Lobby Server
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the GNU General Public License Version 3.
// Text is provided in LICENSE.
//

#ifndef BIGFILE_READFILEOP_H
#define BIGFILE_READFILEOP_H

#include <bigfile/BigArchive.h>
#include <bigfile/RefPack.h>
#include <bigfile/structs/BigStructs.h>

#include <iostream>

#include "../SeekyWheelyAutomobiley.h"

namespace bigfile::detail {

	/**
	 * Lazy file reader operation.
	 */
	struct ReadFileOp {
		explicit ReadFileOp(std::istream& is, BigArchive::File& file)
			: is(is), file(file) {
		}

		bool operator()() {
			auto last_offset = is.tellg();

			is.seekg(file.offset, std::istream::beg);

			switch(file.type) {
				case BigArchive::File::PackType::Uncompressed:
					file.data.resize(file.size);
					if(!is.read(reinterpret_cast<char*>(&file.data[0]), file.size))
						return false;
					break;

				case BigArchive::File::PackType::Refpack: {
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

	   private:
		std::istream& is;
		BigArchive::File& file;
	};

} // namespace bigfile::detail

#endif // BIGFILE_READFILEOP_H
