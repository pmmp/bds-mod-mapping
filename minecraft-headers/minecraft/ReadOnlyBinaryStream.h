#pragma once

#include <string>
#include <memory>
#include "serialize.h"

struct ReadOnlyBinaryStream {
	unsigned long offset;
	char filler[8]; //unknown field
	std::string ownedBuffer;
	std::string& buffer;

	virtual ~ReadOnlyBinaryStream();

	template<typename T>
	T getType() {
		return std::move(serialize<T>::read(*this));
	}

	ReadOnlyBinaryStream(std::string const& buffer, bool copy);
	ReadOnlyBinaryStream(std::string&& buffer);
};

static_assert(offsetof(ReadOnlyBinaryStream, buffer) == 48);
static_assert(sizeof(ReadOnlyBinaryStream) == 56);
