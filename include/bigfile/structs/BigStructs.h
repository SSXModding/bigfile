//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

#ifndef BIGFILE_BIGSTRUCTS_H
#define BIGFILE_BIGSTRUCTS_H

#include <bigfile/UInt24.h>
#include <bigfile/UtilityFunctions.h>

#include <cstdint>
#include <string>
#include <vector>

namespace bigfile {

	/**
	 * BIGF file header
	 */
	struct BigFileHeader {
		std::uint32_t Magic; // BIGF. This is also applicable partially for big4, since BIG<N> follows the same format
		std::uint32_t ArchiveSize;
		std::uint32_t FileCount;
		std::uint32_t IndexTableSize;

		void Read(std::istream& is) {
			is.read(reinterpret_cast<char*>(&Magic), sizeof(Magic));
			is.read(reinterpret_cast<char*>(&ArchiveSize), sizeof(ArchiveSize));
			is.read(reinterpret_cast<char*>(&FileCount), sizeof(FileCount));
			is.read(reinterpret_cast<char*>(&IndexTableSize), sizeof(IndexTableSize));

			SwapBE(ArchiveSize);
			SwapBE(FileCount);
			SwapBE(IndexTableSize);
		}
	};

	/**
	 * BIGF per-file header
	 */
	struct BigTocHeader {
		std::uint32_t Offset;
		std::uint32_t Length;
		std::string Filename;

		void Read(std::istream& is) {
			is.read(reinterpret_cast<char*>(&Offset), sizeof(Offset));
			is.read(reinterpret_cast<char*>(&Length), sizeof(Length));

			SwapBE(Offset);
			SwapBE(Length);

			while(true) {
				char c = is.get();
				if(c == '\0')
					break;

				Filename += c;
			}
		}
	};

	// C0FB header
	struct CoFbFileHeader {
		std::uint16_t Magic;
		std::uint16_t IndexTableSize;
		std::uint16_t FileCount;

		void Read(std::istream& is) {
			is.read(reinterpret_cast<char*>(&Magic), sizeof(Magic));
			is.read(reinterpret_cast<char*>(&IndexTableSize), sizeof(IndexTableSize));
			is.read(reinterpret_cast<char*>(&FileCount), sizeof(FileCount));

			SwapBE(IndexTableSize);
			SwapBE(FileCount);
		}
	};

	/**
	 * Index header
	 */
	struct CoFbTocHeader {
		uint24_le_t Offset;
		uint24_le_t Length;
		std::string Filename;

		void Read(std::istream& is) {
			is.read(reinterpret_cast<char*>(&Offset), sizeof(Offset));
			is.read(reinterpret_cast<char*>(&Length), sizeof(Length));

			while(true) {
				char c = is.get();
				if(c == '\0')
					break;

				Filename += c;
			}
		}
	};

} // namespace bigfile

#endif // BIGFILE_BIGSTRUCTS_H