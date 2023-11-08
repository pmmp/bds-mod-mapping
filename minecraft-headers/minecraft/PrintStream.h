#pragma once

#include <string>

struct PrintStream {
	virtual ~PrintStream(void) {}
	virtual void print(std::string const& string) const = 0;

};
