//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

#ifndef BIGFILE_READFILEOP_H
#define BIGFILE_READFILEOP_H

#include <iosfwd>

#include "../SeekyWheelyAutomobiley.h"
#include <bigfile/BigArchive.h>
#include <bigfile/RefPack.h>
#include <bigfile/structs/BigStructs.h>

namespace bigfile::detail {

	/**
	 * Lazy file reader operation.
	 */
	struct ReadFileOp {
		explicit ReadFileOp(std::istream& is, BigArchive::File& file)
			: is(is), file(file) {
		}

		bool operator()();

	   private:
		std::istream& is;
		BigArchive::File& file;
	};

} // namespace bigfile::detail

#endif // BIGFILE_READFILEOP_H
