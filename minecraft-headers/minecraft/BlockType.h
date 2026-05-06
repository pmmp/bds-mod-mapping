#pragma once

#include "HashedString.h"

struct Block;

struct NewBlockID {
	NewBlockID(const NewBlockID&);

	unsigned short value;
};

struct BlockType {
	char paddingParent[184]; //parent struct

	HashedString fullName;

	char padding[776];

	std::string getFullName() const {
		return fullName.str;
	}

	float getLightEmission(void) const;
	float getTranslucency(void) const;
	float getDestroySpeed() const;
	float getExplosionResistance(void) const;
	float getFriction(void) const;
	Block* getDefaultState() const;

	virtual ~BlockType();

};
