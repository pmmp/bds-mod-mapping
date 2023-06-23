#pragma once

#include "HashedString.h"

struct Block;

struct NewBlockID {
	NewBlockID(const NewBlockID&);

	unsigned short value;
};

struct BlockLegacy {

	char paddingParent[32]; //parent struct

	std::string descriptionId;
	HashedString baseName;
	std::string namespaceName;
	HashedString fullName;

	char padding[172];
	char unknown;
	char opacity;
	char brightness;
	int flameEncouragement;
	int flammability;
	bool lavaFlammable;
	float hardness;
	float blastResistance;
	char padding2[16];
	float friction;

	char padding3[632];

	Block *getStateFromLegacyData(unsigned short) const;
	const NewBlockID getBlockID() const;

	std::string getFullName() const {
		return fullName.str;
	}

	float getDestroySpeed() const;

	virtual ~BlockLegacy();

};

//static_assert(offsetof(BlockLegacy, opacity) == 364);
