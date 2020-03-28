// bigfile: a header-only C++ library for
//			reading data from Electronic Arts
//			BIG archives.
//
// (c) 2020 Lily <lily.modeco80@protonmail.ch> under the terms of the MIT License.

// Implemntation of utlity functions used in bigfile.

namespace detail {

	bool MagicCompare(char value[4]) {
		constexpr static char magic[] = "BIGF";

		if(!strncmp(value, magic, 4))
			return true;

		return false;
	}

}
