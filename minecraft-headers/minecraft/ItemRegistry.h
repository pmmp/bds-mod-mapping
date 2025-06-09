#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include "HashedString.h"
#include "Memory.h"
#include "ItemTag.h"
#include "Item.h"


struct ItemRegistry : std::enable_shared_from_this<ItemRegistry>{
	struct ItemAlias{
		HashedString alias;
		char padding[120];
	};

	struct ItemHashAlias {
		uint64_t aliasName;
		char padding[120]; //BaseGameVersion crap
	};
	struct ComplexAlias {
		std::function<HashedString (short)> mCallback;
		std::vector<std::reference_wrapper<HashedString const>> mSplitNames;
	};

	char padding[40]; // mCerealContext + mValidator
	std::vector<SharedPtr<Item>> mItemRegistry;

	std::unordered_map<int, WeakPtr<Item>> mIdToItemMap;
	std::unordered_map<HashedString, WeakPtr<Item>> mNameToItemMap;
	std::unordered_map<HashedString, WeakPtr<Item>> mTileNamespaceToItemMap;
	std::unordered_map<HashedString, WeakPtr<Item>> mTileItemNameToItemMap;
	std::unordered_map<HashedString, ItemRegistry::ItemAlias> mItemAliasLookupMap;
	std::unordered_map<unsigned long, ItemRegistry::ItemHashAlias> mReverseAliasLookupMap;
	std::unordered_map<unsigned long, ItemRegistry::ItemHashAlias> mReverseFullNameAliasLookupMap;
	std::unordered_map<HashedString, ItemRegistry::ComplexAlias> mComplexAliasLookupMap;
	std::unordered_map<short, HashedString> mLegacyIDToNameMap;
	short mMaxItemID;
	std::vector<HashedString> mAttachableDefinitions;
	std::unordered_map<ItemTag, std::unordered_set<Item const*>> mTagToItemsMap;
};

struct ItemRegistryRef{
	std::weak_ptr<ItemRegistry> mWeakRegistry;
};

struct ItemRegistryManager {
	static void setItemRegistry(const ItemRegistryRef);
	static void resetItemRegistry(void);
	static ItemRegistryRef getItemRegistry(void);
};
