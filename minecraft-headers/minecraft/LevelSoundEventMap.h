#pragma once

#include <string>

namespace SharedTypes::Legacy {
enum class LevelSoundEvent;
}

using LevelSoundEvent = SharedTypes::Legacy::LevelSoundEvent;

struct LevelSoundEventMap{
	static std::string& getName(LevelSoundEvent type);

	static LevelSoundEvent getId(std::string const& string);
};
