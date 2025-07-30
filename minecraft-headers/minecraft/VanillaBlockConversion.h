#pragma once

#include <string>

#include "Memory.h"
#include "Block.h"

namespace VanillaBlockConversion {
	int getBlockTypeLegacyIdFromName(std::string const&);
	Block* tryGetLegacyState(unsigned int, unsigned int);
};
