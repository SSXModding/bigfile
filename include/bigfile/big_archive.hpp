// bigfile: a C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.


// Definition of BigArchive.

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <variant>

#include <bigfile/structures/bigf.hpp>
#include <bigfile/structures/cofb.hpp>


namespace bigfile {

	// TODO: These really should be moved into different files.

	enum class FileType {
		Uncompressed,
		// COFB files are usually packed in Refpack format.
		// In some cases (see SSXFE.BIG which is a COFB archive's tricky.ser) this is not true.
		Refpack
	};

	struct File {
		FileType type;
		std::variant<BigFile, CoFbFile> file;
	};

	struct BigArchive {

		// Default empty constructor.
		BigArchive() { }

		// constructor to auto read from stream
		inline BigArchive(std::istream* stream) {
			ReadFrom(stream);
		}

		// Destructor
		~BigArchive();

		// Read in an archive from a stream
		bool ReadFrom(std::istream* stream);

		// Gets a file from the archive.
		// Will not fetch if already fetched previously.
		File& GetFile(const std::string& path);

		std::vector<std::string> GetPaths();

		inline ArchiveType GetArchiveType() const { return type; }

	private:
		std::map<std::string, File> files;

		ArchiveType type;
		
		
		std::istream* read_stream;
		bool reading = false;
	};
	
}
