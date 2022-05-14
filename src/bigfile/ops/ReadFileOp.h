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
		/**
		 * Constructor.
		 *
		 * \param[in] is Stream to read from
		 * \param[in] file File to read.
		 */
		explicit ReadFileOp(std::istream& is, BigArchive& archive, BigArchive::File& file)
			: is(is), archive(archive), file(file) {
		}

		/**
		 * Execute this operation.
		 * Automatically decompresses RefPack data.
		 *
		 * \return True if success, false otherwise.
		 */
		bool operator()();

	   private:
		/**
		 * The IOStream we will read file data from.
		 */
		std::istream& is;

		/**
		 * The file we should read.
		 */
		BigArchive::File& file;

		BigArchive& archive;
	};

} // namespace bigfile::detail

#endif // BIGFILE_READFILEOP_H
