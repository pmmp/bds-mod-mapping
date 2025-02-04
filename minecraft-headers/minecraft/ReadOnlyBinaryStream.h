#pragma once

#include <string>
#include <memory>
#include "serialize.h"

struct ReadOnlyBinaryStream {
	std::string ownedBuffer;
	std::string_view view;
	size_t offset;
	bool hasOverflowed;

	virtual ~ReadOnlyBinaryStream();

	template<typename T>
	T getType() {
		return std::move(serialize<T>::read(*this));
	}

	ReadOnlyBinaryStream(std::string const& buffer, bool copy);
	ReadOnlyBinaryStream(std::string&& buffer);
};

static_assert(sizeof(ReadOnlyBinaryStream) == 64);
