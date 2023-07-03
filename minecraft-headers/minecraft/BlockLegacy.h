#pragma once

#include "HashedString.h"

struct Block;

struct NewBlockID {
	NewBlockID(const NewBlockID&);

	unsigned short value;
};

struct BlockLegacy {

	std::string descriptionId;
	HashedString baseName;
	std::string namespaceName;
	HashedString fullName;

	Block *getBlockStateFromLegacyData(unsigned char) const;
	Block *getStateFromLegacyData(unsigned short) const;
	const NewBlockID getBlockID() const;

	std::string getFullName() const {
		return fullName.str;
	}

	float getDestroySpeed() const;

	virtual ~BlockLegacy();

};
