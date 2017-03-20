#pragma once

#include <reference.hpp>

namespace Kyrys {
namespace Utils {
class Random {
public:
	Random();
	QString GetRandomString() const;

	static unsigned long Hash(const char *str);

private:
	static QString possibleCharacters;

	unsigned long mix(unsigned long a, unsigned long b, unsigned long c) ;
};
}
}

