//
// SSX 3 Lobby Server
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the GNU General Public License Version 3.
// Text is provided in LICENSE.
//

#ifndef BIGFILE_WRITEHEADEROP_H
#define BIGFILE_WRITEHEADEROP_H

#include <bigfile/BigArchive.h>
#include <bigfile/structs/BigStructs.h>

#include "../SeekyWheelyAutomobiley.h"

namespace bigfile::detail {

	/**
	 * Generic operation to write file header.
	 *
	 * \tparam TFileHeader File header type.
	 */
	template <class TFileHeader>
	struct WriteHeaderOp {
		WriteHeaderOp(BigArchive& archive, std::ostream& os, const TFileHeader& header)
			: archive(archive),
			  os(os),
			  header(header) {
		}

		bool operator()() const {

			return true;
		}

	   private:
		BigArchive& archive;
		std::ostream& os;
		const TFileHeader& header;
	};

} // namespace bigfile::detail
#endif // BIGFILE_WRITEHEADEROP_H
