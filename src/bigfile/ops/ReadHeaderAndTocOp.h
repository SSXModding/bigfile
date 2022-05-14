//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

#include <iostream>

#include "../SeekyWheelyAutomobiley.h"
#include <bigfile/BigArchive.h>
#include <bigfile/structs/BigStructs.h>

#ifndef BIGFILE_READHEADERANDTOCOP_H
#define BIGFILE_READHEADERANDTOCOP_H

namespace bigfile::detail {

	/**
	 * Generic operation to read the header and TOC.
	 * Could technically be made a bit lazier.
	 *
	 * \tparam TFileHeader File header type.
	 * \tparam TTocHeader TOC header type.
	 */
	template <class TFileHeader, class TTocHeader>
	struct ReadHeaderAndTocOp {

		explicit ReadHeaderAndTocOp(std::istream& is, BigArchive& archive)
			: is(is),
				archive(archive) {
		}

		bool operator()() {
			TFileHeader header {};
			header.Read(is);

			if(!is)
				return false;

			// Try to read in Lumpy debug information
			// if it exists. This can reveal:
			//
			// - What version of Lumpy originally packed the archive
			// - What flags were used (e.g: -pack1 -24 or such)
			{
				SeekyWheelyAutomobiley rewinder(is);
				LumpyDebugInfo debugInfo{};

				is.seekg(header.TocSize - sizeof(LumpyDebugInfo), std::istream::beg);

				debugInfo.Read(is);

				if(std::isalpha(debugInfo.LumpyVersion[0])) {
					if(std::isdigit(debugInfo.LumpyVersion[1]) && std::isdigit(debugInfo.LumpyVersion[2]) && std::isdigit(debugInfo.LumpyVersion[3])) {
						// Debug info is valid.
						archive.debugInfo = debugInfo;
					}
				}

			}

			for(auto i = 0; i < header.FileCount; ++i) {
				TTocHeader fileTocEntry {};
				BigArchive::File file;
				fileTocEntry.Read(is);

				file.offset = fileTocEntry.Offset;

				if(!is)
					break;

				// Check if this file is RefPack packed.
				// If so, set the packtype appropriately.
				{
					SeekyWheelyAutomobiley rewinder(is);
					std::uint8_t refpack_test[sizeof(std::uint16_t)];

					is.seekg(fileTocEntry.Offset, std::istream::beg);
					is.read(reinterpret_cast<char*>(&refpack_test[0]), sizeof(std::uint16_t));

					if(refpack_test[0] == 0x10 && refpack_test[1] == 0xFB) {
						uint24_be_t decompressed_size;

						// If the archive hasn't been marked with a pack type,
						// do so now.
						if(archive.packType != PackType::RefPack)
							archive.packType = PackType::RefPack;

						file.compressed_size = fileTocEntry.Length;

						// Fetch decompressed size from the RefPack header,
						// so we don't have to later. Also allows lazier bigpaths!
						if(refpack_test[0] & 0x0100)
							is.seekg(sizeof(uint24_le_t), std::istream::cur);

						is.read(reinterpret_cast<char*>(&decompressed_size), sizeof(decompressed_size));

						file.size = decompressed_size;
					} else {
						if(archive.packType != PackType::Uncompressed)
							archive.packType = PackType::Uncompressed;
						file.size = fileTocEntry.Length;
					}
				}

				archive.files[fileTocEntry.Filename] = file;
			}

			return true;
		}

	   private:
		std::istream& is;
		BigArchive& archive;
	};


}

#endif // BIGFILE_READHEADERANDTOCOP_H
