#pragma once

#include <string>

namespace Puv::Legacy {
enum class LevelSoundEvent;
}

using LevelSoundEvent = Puv::Legacy::LevelSoundEvent;

struct LevelSoundEventMap{
	static std::string& getName(LevelSoundEvent type);

	static LevelSoundEvent getId(std::string const& string);
};
