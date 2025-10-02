#pragma once

#include "BlockType.h"
#include "CompoundTag.h"

typedef CompoundTag BlockSerializationId;

struct Block {
	float getDestroySpeed() const;
	int getFlameOdds(void) const;
	int getBurnOdds(void) const;

	virtual ~Block() {}

	char padding[104];
	unsigned short data;
	BlockType* blockType;

	char filler2[122];
	BlockSerializationId tag;
	char filler3[32];
};
