//
// SSX 3 Lobby Server
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the GNU General Public License Version 3.
// Text is provided in LICENSE.
//

#ifndef BIGFILE_SEEKYWHEELYAUTOMOBILEY_H
#define BIGFILE_SEEKYWHEELYAUTOMOBILEY_H

#include <iostream>

namespace bigfile::detail {

	/**
	 * Seek rewinder.
	 * Go play Unturned if you don't get the joke.
	 */
	struct SeekyWheelyAutomobiley {
		explicit SeekyWheelyAutomobiley(std::istream& is)
			: is(is) {
			last_offset = this->is.tellg();
		}

		~SeekyWheelyAutomobiley() {
			is.seekg(last_offset, std::istream::beg);
		}

	   private:
		std::istream& is;
		std::size_t last_offset;
	};


}

#endif // BIGFILE_SEEKYWHEELYAUTOMOBILEY_H
