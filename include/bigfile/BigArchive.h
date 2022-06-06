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
			 * Read file data from archive
			 */
			bool ReadFile();

			/**
			 * Clear out file data.
			 * If done with a file's data, this can be used
			 * to get rid of it and save some memory.
			 */
			void Done();

			std::uint32_t GetOffset() const;
			std::uint32_t GetSize() const;

			/**
			 * With compressed files,
			 * returns the compressed size.
			 */
			std::uint32_t GetCompressedSize() const;

			/**
			 * Get file data. Only valid if file is in memory,
			 * or was inserted into the archive.
			 */
			const std::vector<std::uint8_t>& GetData() const;


		   private:
			friend BigArchive;

			template <class TFileHeader, class TTocHeader>
			friend struct detail::ReadHeaderAndTocOp;
			friend struct detail::ReadFileOp;

			explicit File(BigArchive& archive);

			/**
			 * The archive this file object is a part of.
			 */
			BigArchive& parentArchive;


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

		using FileMap = std::unordered_map<std::string, File>;
		using iterator = FileMap::iterator;
		using const_iterator = FileMap::const_iterator;

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

		iterator begin();
		iterator end();

		const_iterator cbegin();
		const_iterator cend();

		File& operator[](const std::string& path);
		const File& operator[](const std::string& path) const;


		iterator insert(const std::string& path, const std::vector<std::uint8_t>& data);

		// do this?
		//iterator insert(const_iterator hint, const std::string& path, const std::vector<std::uint8_t>& data);

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