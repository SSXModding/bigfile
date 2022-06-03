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
#include <bigfile/structs/BigStructs.h>

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace bigfile {

	namespace detail {
		template <class TFileHeader, class TTocHeader>
		struct ReadHeaderAndTocOp;

		struct ReadFileOp;
	}

	struct BigArchive {
		/**
		 * "Generic" file structure
		 */
		struct File {
			/**
			 * Returns true if this file has been loaded into memory.
			 * False otherwise.
			 */
			[[nodiscard]] bool IsInMemory() const;

			/**
			 * Clear out file data.
			 * If done with a file's data, this can be used
			 * to get rid of it and save some memory.
			 */
			void Done();

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

		   private:
			friend BigArchive;

			// Not implemented yet

			//explicit File(BigArchive& archive);

		};

		using FileMap = std::unordered_map<std::string, File>;
		using iterator = FileMap::iterator;
		using const_iterator = FileMap::iterator;

		BigArchive() = default;

		/**
		 * Shorthand constructor to initialize a clean archive.
		 */
		BigArchive(ArchiveType type, PackType packType);

		/**
		 * Initialize a clean archive, used for writing.
		 *
		 * \param[in] type Archive type.
		 * \param[in] packType The archive pack type.
		 */
		void InitArchive(ArchiveType type, PackType packType);

		/**
		 * Read in a BIG archive from the given stream.
		 * This only reads the header and TOC from the archive,
		 * all files are read lazily.
		 *
		 * \returns True if successful, false on any error.
		 */
		bool ReadArchive(std::istream& stream);

		/**
		 * Get the current archive type.
		 */
		[[nodiscard]] ArchiveType GetArchiveType() const;

		/**
		 * Get archive pack type.
		 */
		[[nodiscard]] PackType GetPackType() const;

		/**
		 * Get debug info (if it exists)
		 */
		[[nodiscard]] std::optional<LumpyDebugInfo> GetDebugInfo() const;

		// Iterator API (not implemented yet)
#if 0
		iterator begin();
		iterator end();

		const_iterator cbegin();
		const_iterator cend();

		File& operator[](const std::string& path);
		const File& operator[](const std::string& path) const;
#endif

		/**
		 * Get a file from the archive. Returns an optional,
		 * which is empty in case of error.
		 *
		 * \param[in] path BIG file path
		 * \param[in] wantsData Whether or not you want file data. For a ls-like program, or basic metadata,
		 * 						 you might not. Defaults to true (causing a lazy-read if not read yet.)
		 */
		[[nodiscard, deprecated("Use iterator APIs where possible.")]] std::optional<std::reference_wrapper<File>> GetFile(const std::string& path, bool wantsData = true);

		/**
		 * Get all paths stored in the BIG archive.
		 */
		[[nodiscard, deprecated("Use iterator APIs where possible.")]] std::vector<std::string> GetPaths();


	   private:

		template <class TFileHeader, class TTocHeader>
		friend struct detail::ReadHeaderAndTocOp;
		friend struct detail::ReadFileOp;
		friend struct File;

		FileMap files;

		ArchiveType type{};
		PackType packType{};

		std::optional<LumpyDebugInfo> debugInfo{};

		/**
		 * Input stream reference.
		 * Only needs to be valid when told to read.
		 */
		std::optional<std::reference_wrapper<std::istream>> inputStream{};
	};

} // namespace bigfile

#endif // BIGFILE_BIGARCHIVE_H