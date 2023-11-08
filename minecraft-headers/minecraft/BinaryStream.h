#pragma once

#include "ReadOnlyBinaryStream.h"
#include "serialize.h"

struct BinaryStream : ReadOnlyBinaryStream {
	std::string unk11;
	std::string* unk2;

	BinaryStream();

	void write(void const *, unsigned long);

	void writeUnsignedShort(unsigned short);

	void writeUnsignedVarInt(unsigned int);

	template<typename T>
	void writeType(const T &in) {
		serialize<T>::write(in, *this);
	}
};

static_assert(sizeof(BinaryStream) == 88); //correct with libc++ in 1.20.40
