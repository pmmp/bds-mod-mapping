#pragma once
#include <functional>
#include <map>
#include "Block.h"
#include "HashedString.h"
#include "Memory.h"

struct BlockTypeRegistry{
	static const Block * lookupByName(const HashedString &, int, bool) const;

	static std::map<HashedString, SharedPtr<Block>> mBlockTypeRegistry;
};
