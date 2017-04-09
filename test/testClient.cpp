//"Catch test" - testing framework header files
#include "catch.hpp"

//Kyrys project header files
#include <reference.hpp>
#include <enums.hpp>
#include <client.hpp>

//Standard c++ library header files
#include <iostream>

using Kyrys::Client;

using std::string;
using std::istringstream;
using std::endl;
using std::cout;

//For more info check: http://en.cppreference.com/w/cpp/string/basic_string/getline
TEST_CASE("Client - loadLoginCredentials - automatic istream demonstration"){
	cout << "Client - loadLoginCredentials - automatic istream demonstration" << endl;

	using Kyrys::Enums::Client::Login::Status;
	istringstream in;

	string nicknameControl = "random nickname";
	string passwordControl = "random password";
	string nicknameBuffer;
	string passwordBuffer;

	in.str(nicknameControl + "\n" + passwordControl + "\n"); //this is prepared fake input

	Client client("localhost", 12345);
	int returnValue = client.loadLoginCredentials(nicknameBuffer, passwordBuffer, in);
	REQUIRE(returnValue == Status::SUCCESS);
	REQUIRE(nicknameControl == nicknameBuffer);
	REQUIRE(passwordControl == passwordBuffer);

	cout << "\n\n" << endl;
}