#pragma once

#include "BlockType.h"
#include "CompoundTag.h"

typedef CompoundTag BlockSerializationId;

struct Block {
	float getDestroySpeed() const;
	int getFlameOdds(void) const;
	int getBurnOdds(void) const;

	virtual ~Block() {}

	char filler1[80];
	BlockType* blockType; // 88-96
	char filler2[128];
	BlockSerializationId tag; // 224-256
	char filler3[24];
	unsigned short data; // 280-282
	char filler4[6];
};
