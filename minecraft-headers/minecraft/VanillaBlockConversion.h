#pragma once

#include <string>

#include "Block.h"
#include "Memory.h"

namespace VanillaBlockConversion {
	int getBlockTypeLegacyIdFromName(std::string const&);
	Block* tryGetLegacyState(unsigned int, unsigned int);
};
