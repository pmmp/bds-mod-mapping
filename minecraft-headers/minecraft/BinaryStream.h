#pragma once

#include "ReadOnlyBinaryStream.h"
#include "serialize.h"

struct BinaryStream : ReadOnlyBinaryStream {
	std::string& buffer;

	BinaryStream();

	void write(void const *, unsigned long);

	void writeUnsignedShort(unsigned short, char const*, char const*);

	void writeUnsignedVarInt(unsigned int, char const*, char const*);

	template<typename T>
	void writeType(const T &in, char const*, char const*) {
		serialize<T>::write(in, *this);
	}
};

static_assert(offsetof(BinaryStream, buffer) == 64);
static_assert(sizeof(BinaryStream) == 72);
