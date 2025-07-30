#pragma once

#include <string>

#include "Block.h"

struct BlockPalette {
	const Block *getBlock(BlockSerializationId const &) const;
	const Block *getBlock(const unsigned int & blockRuntimeId) const;

	size_t getNumBlockNetworkIds() const;
};
