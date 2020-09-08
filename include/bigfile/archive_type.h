#pragma once

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