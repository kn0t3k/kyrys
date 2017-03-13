#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>

#include <client.hpp>

TEST_CASE("fooTest"){
	std::cout << "FooTest" << std::endl;
	
	Foo();

	REQUIRE(0 == 0);

}
