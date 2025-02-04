#pragma once

#include "HashedString.h"

struct Biome {
	HashedString name;
	char filler[354];
	unsigned short biomeId;

	virtual ~Biome(void);
};
