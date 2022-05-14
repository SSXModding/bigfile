//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

#ifndef BIGFILE_SEEKYWHEELYAUTOMOBILEY_H
#define BIGFILE_SEEKYWHEELYAUTOMOBILEY_H

#include <iostream>

namespace bigfile::detail {

	/**
	 * Stream rewinder.
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
