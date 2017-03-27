#pragma once

#include <reference.hpp>

namespace Kyrys {
namespace Utils {
class Random {
public:
	/**
	 * @brief      Construt a random object - initialize random generator.
	 */
	Random();

	/**
	 * @brief      Create a random string.
	 *
	 * @return     The random string.
	 */
	QString GetRandomString() const;

private:
	static QString possibleCharacters;

	unsigned long mix(unsigned long a, unsigned long b, unsigned long c) ;
};
}
}

