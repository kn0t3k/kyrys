#include "catch.hpp"

#include <iostream>

TEST_CASE("barTest"){
	std::cout << "BarTest" << std::endl;
	REQUIRE(0 == 0);
}
