// bigfile: a header-only C++ library for
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


namespace detail {

	struct BigArchive {

		// Default empty constructor.
		BigArchive() { }

		// constructor to auto read from stream
		inline BigArchive(std::shared_ptr<std::istream> stream) {
			ReadFrom(stream);
		}

		// Destructor
		~BigArchive();

		// Read in an archive from a stream
		bool ReadFrom(std::shared_ptr<std::istream> stream);

		// Gets a file from the archive.
		// Will not fetch if already fetched previously.
		BigFile& GetFile(const std::string& path);

		std::vector<std::string> GetPaths();

		// Opens a nested archive.
		// Creates a brand new BigArchive object ready for use.
		std::shared_ptr<BigArchive> GetNestedArchive(const std::string& path);


	private:
		std::map<std::string, BigFile> files;
		BigHeader header;
		
		std::shared_ptr<std::istream> read_stream;
		bool reading = false;
	};
	
}
