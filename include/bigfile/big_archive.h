#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <variant>
#include <optional>

#include <bigfile/types.h>
#include <bigfile/archive_type.h>

namespace bigfile {

	// TODO:
	// - move the file structure into another file we include here
	// - start work on implementing serialization & verify with game assets (this will be reeeal fun)

	/**
	 * file type for C0FB files
	 */
	enum class CofbFileType {
		Uncompressed,
		// COFB files are usually packed in Refpack format.
		// In some cases (see SSXFE.BIG which is a COFB archive's tricky.ser) this is not true.
		Refpack
	};

	/**
	 * "Generic" file structure
	 */
	struct File {
		/**
		 * C0FB file type.
		 * 
		 * Unused (defaults to CoFbFileType::Uncompressed) if the archive is purely BIGF.
		 */
		CofbFileType type = CofbFileType::Uncompressed;
		
		/**
		 * Compressed file size.
		 */
		uint32 compressed_size = 0;

		/**
		 * Actual file size.
		 */
		uint32 size = 0;

		/**
		 * File data
		 * Automatically decompressed if the file is refpack compressed as a C0FB
		 */
		std::vector<byte> data;
	};

	struct BigArchive {
		BigArchive();
		~BigArchive();
		
		/** 
		 * Read in a BIGF or C0FB archive from the given stream.
		 * Returns false on error.
		 */
		bool ReadFrom(std::istream& stream);

		
		/** 
		 * Get a file from the archive. Returns an optional,
		 * which is empty in case of error
		 */
		std::optional<File> GetFile(const std::string& path);

		std::vector<std::string> GetPaths();

		/** 
		 * Get the current archive type. 
		 */
		ArchiveType GetCurrentArchiveType();

		// Archive Writing functions
		// Not yet implemented

		/**
		 * Set up an new empty archive.
		 * This will clean out ANY archive already loaded
		 * so do not do this for replacement
		 */
		bool SetupArchive(ArchiveType type);

		/** 
		 * Add a file with the given path (actually a filename), with the given CoFb file type.
		 * NOTE: the file type is ignored if the archive type is BIGF.
		 */
		bool AddFile(const std::string& path, File& file, CofbFileType type = CofbFileType::Refpack);

		/**
		 * Like AddFile(), but only functions if the path exists beforehand
		 * and remembers the previous file type.
		 */
		bool ReplaceFile(const std::string& path, File& file);

		// Remove a file from the archive.
		bool RemoveFile(const std::string& path);

		// Serialize the archive to the given stream.
		bool WriteArchive(std::ofstream& stream);

	   private:
		   struct Impl;
		   Impl* impl;
	};

} // namespace bigfile
