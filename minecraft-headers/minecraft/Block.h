#pragma once

#include "BlockLegacy.h"
#include "CompoundTag.h"

typedef CompoundTag BlockSerializationId;

struct Block {
	const BlockLegacy & getLegacyBlock() const {
		return *blockLegacy;
	}

	float getDestroySpeed() const;

	virtual ~Block() {}

	unsigned short data;
	BlockLegacy* blockLegacy;

	char filler2[128];
	BlockSerializationId tag;
};
