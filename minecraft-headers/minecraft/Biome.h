#pragma once

#include "HashedString.h"

struct Biome {
	HashedString name;
	char filler[114];
	unsigned short biomeId;

	virtual ~Biome(void);
};
