#pragma once

#include "BlockType.h"
#include "CompoundTag.h"

typedef CompoundTag BlockSerializationId;

struct Block {
	float getDestroySpeed() const;

	virtual ~Block() {}

	char padding[104];
	unsigned short data;
	BlockType* blockType;

	char filler2[130];
	BlockSerializationId tag;
	char filler3[32];
};
