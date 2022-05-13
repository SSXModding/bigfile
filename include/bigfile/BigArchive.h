//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

#ifndef BIGFILE_BIGARCHIVE_H
#define BIGFILE_BIGARCHIVE_H

#include <bigfile/ArchiveType.h>

#include <functional>
#include <iosfwd>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace bigfile {

	// TODO:
	// - start work on implementing serialization & verify with game assets (this will be reeeal fun)

	struct BigArchive {
		/**
		 * "Generic" file structure
		 */
		struct File {
			enum class PackType {
				Uncompressed,
				Refpack
			};

			/**
			 * Returns true if this file has been loaded into memory.
			 * False otherwise.
			 */
			bool IsInMemory() const;

			/**
			 * Clear out file data.
			 * If done with a file's data, this can be used
			 * to get rid of it and save some memory.
			 */
			void Done();

			/**
			 * File pack type.
			 */
			PackType type { PackType::Uncompressed };

			/**
			 * Compressed file size.
			 */
			std::uint32_t compressed_size {};

			/**
			 * Actual file size.
			 */
			std::uint32_t size {};

			/**
			 * Offset.
			 */
			std::uint32_t offset {};

			/**
			 * File data,
			 * Automatically decompressed if the file is refpack compressed as a C0FB.
			 */
			std::vector<std::uint8_t> data {};
		};

		/**
		 * Read in a BIGF or C0FB archive from the given stream.
		 * Returns false on any error.
		 */
		bool ReadToc(std::istream& stream);

		/**
		 * Get a file from the archive. Returns an optional,
		 * which is empty in case of error
		 */
		std::optional<std::reference_wrapper<File>> GetFile(const std::string& path, bool wantsData = true);

		/**
		 * Get all paths stored in the BIG archive.
		 */
		std::vector<std::string> GetPaths();

		/**
		 * Get the current archive type.
		 */
		ArchiveType GetArchiveType();

	   private:
		std::map<std::string, File> files;
		ArchiveType type;

		std::optional<std::reference_wrapper<std::istream>> inputStream;
	};

} // namespace bigfile

#endif // BIGFILE_BIGARCHIVE_H