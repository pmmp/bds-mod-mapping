#pragma once

#include "CompoundTag.h"

typedef CompoundTag BlockSerializationId;

struct Block {
	std::string getRawNameId() const;
	float getLightEmission() const;
	float getTranslucency() const;
	int getBurnOdds() const;
	int getFlameOdds() const;
	float getDestroySpeed() const;
	float getExplosionResistance() const;
	float getFriction() const;

	virtual ~Block() {}

	char padding[112];
	unsigned short data;

	char filler2[150];
	BlockSerializationId tag;
	char filler3[24];
};
