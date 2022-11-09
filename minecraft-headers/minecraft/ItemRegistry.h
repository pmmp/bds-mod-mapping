#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
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

    static std::unordered_map<ItemTag, std::unordered_set<Item const *, std::hash<Item const *>, std::equal_to<Item const *>, std::allocator<Item const *> >, std::hash<ItemTag>, std::equal_to<ItemTag>, std::allocator<std::pair<ItemTag const, std::unordered_set<Item const *, std::hash<Item const *>, std::equal_to<Item const *>, std::allocator<Item const *> > > > > mTagToItemsMap;

    static std::pair<HashedString, int> getNameFromAlias(HashedString const & name, int metadata);

};
