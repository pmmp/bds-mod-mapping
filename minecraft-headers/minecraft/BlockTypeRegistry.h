#pragma once
#include <functional>
#include <map>
#include "Block.h"
#include "BlockType.h"
#include "HashedString.h"
#include "Memory.h"

struct BlockTypeRegistry{
	static BlockTypeRegistry & get(void);

	WeakPtr<BlockType> lookupByName(const HashedString &, bool) const;
	const Block * lookupByName(const HashedString &, int, bool) const;

	std::map<HashedString, SharedPtr<BlockType>> mBlockLookupMap;
};
