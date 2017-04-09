#include "catch.hpp"

#include <reference.hpp>
#include <client.hpp>
#include <enums.hpp>
#include <string>
#include <cstdio>

using Kyrys::Client;



TEST_CASE("Loading login credentials - testing fake input stream from POSIX library") {
	std::string nickname;
	std::string password;
	Client client("localhost", 1);

	std::string controlNickname = "John Doe";
	std::string controlPassword = "UltraSecurePassword";

	//REQUIRE(controlNickname == nickname);
	//REQUIRE(controlPassword == password);
}
