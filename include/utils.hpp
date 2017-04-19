#pragma once

#include <reference.hpp>

namespace Kyrys {
namespace Utils {
class Random {
private:
	static QString m_possibleCharacters;

	unsigned long mix(unsigned long a, unsigned long b, unsigned long c) ;

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
	QString getRandomString(int randomStringLen = 10) const;

};
}
}

