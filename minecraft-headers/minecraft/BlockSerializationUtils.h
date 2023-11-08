#pragma once

#include <utility>
#include "Block.h"
#include "CompoundTag.h"

namespace BlockSerializationUtils {
	struct NbtToBlockCache; //structure not needed for now

	//pair.first is a status code?
	std::pair<int, Block const*> tryGetBlockFromNBT(CompoundTag const&, BlockSerializationUtils::NbtToBlockCache*);

	void clearNBTToBlockCache(void);
};
