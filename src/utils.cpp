#include <utils.hpp>
#include <ctime>

using Kyrys::Utils::Random;

QString Random::m_possibleCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

Random::Random() {
	qsrand(mix(clock(), time(NULL), QTime::currentTime().msec()));
}

QString Random::getRandomString(int randomStringLength) const {
	QString randomString;

	for (int i = 0; i < randomStringLength; ++i) {
		int index = qrand() % m_possibleCharacters.length();

		QChar nextChar = m_possibleCharacters.at(index);
		randomString.append(nextChar);
	}
	return randomString;
}

unsigned long Random::mix(unsigned long a, unsigned long b, unsigned long c) {
	a = a - b;  a = a - c;  a = a ^ (c >> 13);
	b = b - c;  b = b - a;  b = b ^ (a << 8);
	c = c - a;  c = c - b;  c = c ^ (b >> 13);
	a = a - b;  a = a - c;  a = a ^ (c >> 12);
	b = b - c;  b = b - a;  b = b ^ (a << 16);
	c = c - a;  c = c - b;  c = c ^ (b >> 5);
	a = a - b;  a = a - c;  a = a ^ (c >> 3);
	b = b - c;  b = b - a;  b = b ^ (a << 10);
	c = c - a;  c = c - b;  c = c ^ (b >> 15);
	return c;
}
