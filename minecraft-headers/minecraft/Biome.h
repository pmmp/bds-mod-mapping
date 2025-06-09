#pragma once

#include "HashedString.h"

struct Biome {
	char filler[354];
	unsigned short biomeId;
	char filler2[32]; //mBiomeComponentStorage
	HashedString name;

	virtual ~Biome(void);
};
