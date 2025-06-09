#pragma once
#include <functional>
#include <map>
#include "Block.h"
#include "BlockLegacy.h"
#include "HashedString.h"
#include "Memory.h"

struct BlockTypeRegistry{
	static WeakPtr<BlockLegacy> lookupByName(const HashedString &, bool);
	static const Block * lookupByName(const HashedString &, int, bool);

	static std::map<HashedString, SharedPtr<BlockLegacy>> mBlockLookupMap;
};
