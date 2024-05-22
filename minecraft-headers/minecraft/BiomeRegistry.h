#pragma once

#include <functional>
#include "Biome.h"

struct BiomeRegistry {
	Biome* lookupById(int) const;

	void forEachBiome(std::function<void(Biome const&)>) const;
};
