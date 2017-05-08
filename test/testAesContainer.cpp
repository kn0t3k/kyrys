//"Catch test" - testing framework header files
#include "catch.hpp"

//Kyrys project header files
#include <reference.hpp>
#include <AesContainer.hpp>

//STD Library
#include <cstring>

#define CONTAIN 1 //This macro is enabling printing of atribute's contain after calling tested method

using Kyrys::AesContainer;

using std::string;
using std::endl;
using std::cout;

TEST_CASE("AesContainer - Constructor"){

	if(CONTAIN)cout << 	"\n"
						"+----------------------------------+\n"
						"|  AesContainer - Constructor test |\n"
						"+----------------------------------+" << endl;

	unsigned char key[] = {'T', 'h', 'i', 's',
						   '_', 'i', 's', '_',
						   'a', '_', 'k', 'e',
						   'y', '@', '.', '*'};
	string message = "Hello world";
	unsigned char defaultVector[16] = {35 , 34  , 100 , 35,
									   81 , 125 , 77  , 40,
									   51 , 55  , 42  , 71,
									   111, 70  , 77  , 101}; //for testing, use values from interval <33, 126> !
	AesContainer aes(key, defaultVector, message, 128);

	//Contain of basic input values
	if(CONTAIN) {
		std::cout << "\n"
					 "Basic input values\n"
				     "------------------\n"
				     "m_data 		  : " << aes.getData() << "\n" <<
				     "m_input_length    : " << aes.getInput_length() << "\n" <<
				     "m_paddingLength   : " << aes.getPaddingLength() << "\n" <<
				     "m_output_length   : " << aes.getOutput_length() << std::endl;
		std::cout << "m_iv (char) 	  : "; aes.printArray(aes.getIV(), 16, "char"); 	std::cout << std::endl;
		std::cout << "m_iv (int) 	  : "; aes.printArray(aes.getIV(), 16, "int"); 		std::cout << std::endl;
		std::cout << "m_key (char) 	  : "; aes.printArray(aes.getKey(), 16, "char"); 	std::cout << std::endl;
		std::cout << "m_key (int) 	  : "; aes.printArray(aes.getKey(), 16, "int"); 	std::cout << std::endl;
		std::cout << "m_version 	  : " << aes.getVersion() << "\n" << std::endl;
	}

	//Test of basic input values
	REQUIRE(message == aes.getData());
	REQUIRE(std::memcmp(key, aes.getKey(), 16) == 0);
	REQUIRE(std::memcmp(defaultVector, aes.getIV(), 16) == 0);
	REQUIRE(128 == aes.getVersion());

	//Test of other properties - length of array atributes
	REQUIRE(message.length() == aes.getInput_length());
	REQUIRE(message.length() + (128 - (message.length() % aes.getVersion())) == aes.getOutput_length());
	REQUIRE(aes.getPaddingLength() < aes.getVersion());

	//Test of another message longer than one block of 128B
	message = "Hello world, this is very long message. Message longer than block of 128B. I hope that encryption will finish OK. And because previous text is too short, I need to add some more of it.";
	AesContainer aes2(key, defaultVector, message, 128);

	//Contain of basic input values
	if(CONTAIN) {
		std::cout << "\n"
				"Basic input values\n"
				"------------------\n"
				"m_data 		  : " << aes2.getData() << "\n" <<
				  "m_input_length    : " << aes2.getInput_length() << "\n" <<
				  "m_paddingLength   : " << aes2.getPaddingLength() << "\n" <<
				  "m_output_length   : " << aes2.getOutput_length() << std::endl;
		std::cout << "m_iv (char) 	  : "; aes2.printArray(aes.getIV(), 16, "char"); 	std::cout << std::endl;
		std::cout << "m_iv (int) 	  : "; aes2.printArray(aes.getIV(), 16, "int"); 		std::cout << std::endl;
		std::cout << "m_key (char) 	  : "; aes2.printArray(aes.getKey(), 16, "char"); 	std::cout << std::endl;
		std::cout << "m_key (int) 	  : "; aes2.printArray(aes.getKey(), 16, "int"); 	std::cout << std::endl;
		std::cout << "m_version 	  : " << aes2.getVersion() << "\n" << std::endl;
	}

	//Test of basic input values
	REQUIRE(message == aes2.getData());
	REQUIRE(std::memcmp(key, aes2.getKey(), 16) == 0);
	REQUIRE(std::memcmp(defaultVector, aes2.getIV(), 16) == 0);
	REQUIRE(128 == aes2.getVersion());

	//Test of other properties - length of array atributes
	REQUIRE(message.length() == aes2.getInput_length());
	REQUIRE(message.length() + (128 - (message.length() % aes2.getVersion())) == aes2.getOutput_length());
	REQUIRE(aes2.getPaddingLength() < aes2.getVersion());
}


TEST_CASE("AesContainer - padding"){

}


TEST_CASE("AesContainer - encryption / decryption"){

}