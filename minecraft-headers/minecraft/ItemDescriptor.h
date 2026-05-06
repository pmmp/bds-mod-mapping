#pragma once

#include "Block.h"
#include "BlockType.h"
#include "Item.h"

class ItemDescriptor {
	private:
		void *impl;

	public:
		ItemDescriptor(void);
		ItemDescriptor(const Block &);
		ItemDescriptor(const BlockType &);
		ItemDescriptor(const Item &, int);
		ItemDescriptor(const ItemDescriptor &);
		ItemDescriptor(ItemDescriptor &&);
	public:
		virtual ~ItemDescriptor(void);
		void operator=(const ItemDescriptor &);
		void operator=(ItemDescriptor &&);
		const Item * getItem(void) const;
		const Block * getBlock(void) const;
		const WeakPtr<BlockType const> & getBlockType(void) const;
		bool isDefinedAsItemName(void) const;
		bool isValid(bool) const;
		bool isNull(void) const;
		short getId(void) const;
		short getAuxValue(void) const;
		int getIdAux(void) const;
		std::string getRawNameId(void) const;
		std::string getSerializedNameAndAux(void) const;
		std::string getFullName(void) const;
		size_t getHash(void) const;
		bool operator==(const ItemDescriptor &) const;
		bool operator!=(const ItemDescriptor &) const;
};
