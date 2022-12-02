#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include "HashedString.h"
#include "Memory.h"
#include "ItemTag.h"

struct ItemRegistry{
	static std::vector<SharedPtr<Item>> mItemRegistry;

	struct ItemAlias{
		HashedString alias;
		char padding[120];
	};

	static std::unordered_map<HashedString, ItemAlias> mItemAliasLookupMap;
	static std::unordered_map<HashedString, std::function<HashedString(short)>> mComplexAliasLookupMap;

	static std::unordered_map<ItemTag, std::unordered_set<Item const *>> mTagToItemsMap;

	static std::pair<HashedString, int> getNameFromAlias(HashedString const & name, int metadata);

};
