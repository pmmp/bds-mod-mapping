#pragma once

#include <functional>
#include "Biome.h"
#include "FunctionRef.h"

struct BiomeRegistry {
	Biome* lookupById(int) const;

	void forEachBiome(brstd::function_ref<void(Biome const&), void(Biome const&)>) const;
};
