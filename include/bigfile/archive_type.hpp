// bigfile: a C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// ArchiveType enum

namespace bigfile {
	
	enum class ArchiveType {
		// Not an archive
		NotArchive,

		// Conventional BIGF.
		BIGF,

		// Only used (at least, only seen in) by SSX Tricky.
		CoFb
	};
	
}