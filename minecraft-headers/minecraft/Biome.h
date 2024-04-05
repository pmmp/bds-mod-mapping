#pragma once

#include "HashedString.h"

struct Biome {
	HashedString name;
	char filler[72];
	unsigned int biomeId;

	virtual ~Biome(void);
};
