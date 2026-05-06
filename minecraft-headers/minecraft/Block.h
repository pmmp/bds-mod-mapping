#pragma once

#include "BlockType.h"
#include "CompoundTag.h"

typedef CompoundTag BlockSerializationId;

struct Block {
	float getDestroySpeed() const;
	int getFlameOdds(void) const;
	int getBurnOdds(void) const;

	virtual ~Block() {}

	char filler1[96];
	BlockType* blockType; // 104-112
	char filler2[128];
	BlockSerializationId tag; // 240-272
	char filler3[24];
	unsigned short data; // 296-312
	char filler4[6];
};
