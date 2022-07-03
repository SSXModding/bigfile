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
		 * \param[in] archive The archive the file is in. Must have a valid input stream.
		 * \param[in] file File to read.
		 */
		explicit ReadFileOp(BigArchive& archive, BigArchive::File& file)
			: archive(archive), file(file) {
			// Simple fool-check.
			if(!archive.inputStream.has_value())
				throw std::runtime_error("ReadFileOp() attempted to be created without a valid input stream");
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
		 * The file we should read.
		 */
		BigArchive::File& file;

		BigArchive& archive;
	};

} // namespace bigfile::detail

#endif // BIGFILE_READFILEOP_H
