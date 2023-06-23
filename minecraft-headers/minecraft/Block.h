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

	char padding[26];
	unsigned short data;
	BlockLegacy* blockLegacy;

	char filler2[132];
	BlockSerializationId tag;
	char filler3[32];
};
