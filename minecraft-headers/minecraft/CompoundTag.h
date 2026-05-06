#pragma once

#include <string>
#include <string_view>
#include <memory>

#include "Tag.h"
#include "PrintStream.h"

class CompoundTag : public Tag{
public:
	using StringView = std::string_view;

	char filler[24];

	CompoundTag(void);
	CompoundTag(const CompoundTag&);
	CompoundTag(CompoundTag &&);

	CompoundTag* putCompound(std::string name, std::unique_ptr<CompoundTag> tag);
	short& putShort(std::string name, short value);

	std::unique_ptr<CompoundTag> clone(void) const;

	bool equals(const Tag &) const; //this is actually virtual, but this should be fine for our purposes - otherwise we'd have to recreate the whole vtable to get the right offset
	size_t hash(void) const; //again, technically virtual, but doesn't need to be for our purposes

	virtual ~CompoundTag(void);

	void print(std::string const& prefix, PrintStream& stream) const;
	bool remove(StringView);
};
