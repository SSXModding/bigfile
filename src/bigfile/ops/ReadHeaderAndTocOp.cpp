//
// Bigfile
//
// (C) 2020-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// This file is licensed under the MIT License.
//

#include "ReadHeaderAndTocOp.h"

namespace bigfile::detail {

	template struct ReadHeaderAndTocOp<BigFileHeader>;
	template struct ReadHeaderAndTocOp<CoFbFileHeader>;

} // namespace bigfile::detail