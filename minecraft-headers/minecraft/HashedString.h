#pragma once

#include <string>

struct HashedString{
	unsigned long long hash;
	std::string str;
	struct HashedString* wtf;

	HashedString(std::string const & input);
	HashedString(char const * input);
};
static_assert(sizeof(HashedString) == 40);
