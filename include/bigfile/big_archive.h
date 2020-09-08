#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <variant>
#include <optional>

#include <bigfile/archive_type.h>
#include <bigfile/structures/bigf.h>
#include <bigfile/structures/cofb.h>

namespace bigfile {

	// TODO: doxygenate

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
		 * file data
		 * Automatically uncompressed if the file is refpack compressed as a C0FB
		 */
		std::vector<byte> data;
	};

	struct BigArchive {
		// Default empty constructor.
		BigArchive();

		// Destructor
		~BigArchive();

		// Read in an archive from a stream
		bool ReadFrom(std::istream& stream);

		// Gets a file from the archive.
		// Will not fetch if already fetched previously.
		std::optional<File> GetFile(const std::string& path);

		std::vector<std::string> GetPaths();

		// return the archive type
		ArchiveType GetCurrentArchiveType();

		// WRITING FUNCTIONS
		//
		// NOT IMPLEMENTED YET
		//

		// Setup the archive for the given type
		bool SetupArchive(ArchiveType type);

		// Add a file with the given path (actually a filename), with the file type.
		// NOTE: the file type is ignored if the archive type is bigf.
		// If you're writing a tool to replace files,
		// make VERY sure to keep note of the file type.
		bool AddFile(const std::string& path, File& file, CofbFileType type = CofbFileType::Uncompressed);

		// Remove a file from the archive.
		void RemoveFile(const std::string& path);

		// Serialize the archive to the given stream.
		bool WriteArchive(std::ostream& stream);

	   private:
		   struct Impl;
		   Impl* impl;
	};

} // namespace bigfile
