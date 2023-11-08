#pragma once

#include "HashedString.h"

struct Biome {
	HashedString name;
	char filler[80];
	unsigned int biomeId;

	virtual ~Biome(void);
};
