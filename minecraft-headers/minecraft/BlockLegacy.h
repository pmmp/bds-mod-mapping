#pragma once

#include "HashedString.h"

struct Block;

struct NewBlockID {
	NewBlockID(const NewBlockID&);

	unsigned short value;
};

struct BlockLegacy {

	char paddingParent[128]; //parent struct

	HashedString baseName;
	std::string namespaceName;
	HashedString fullName;

	char padding[156];
	char unknown;
	char opacity;
	char brightness;
	int flameEncouragement;
	int flammability;
	bool lavaFlammable;

	Block *getStateFromLegacyData(unsigned short) const;
	const NewBlockID getBlockID() const;

	std::string getFullName() const {
		return fullName.str;
	}

	float getLightEmission(void) const;
	float getTranslucency(void) const;
	int getBurnOdds(void) const;
    int getFlameOdds(void) const;
	float getDestroySpeed() const;
	float getExplosionResistance(void) const;
    float getFriction(void) const;

	virtual ~BlockLegacy();

};

//static_assert(offsetof(BlockLegacy, opacity) == 364);
