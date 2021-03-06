//"Catch test" - testing framework header files
#include "catch.hpp"

//Kyrys project header files
#include <reference.hpp>
#include <enums.hpp>
#include <chatInterface.hpp>

//Standard c++ library header files
#include <iostream>

//Qt framework library
#include <QtCore/QCryptographicHash>


using Kyrys::ChatInterface;

using std::string;
using std::istringstream;
using std::endl;
using std::cout;

typedef Kyrys::Enums::Client::Login::Status lStatus;
typedef Kyrys::Enums::Client::Registration::Status rStatus;
typedef Kyrys::Enums::JsonMessage::MessageType MessageType;

ChatInterface client(nullptr);

//For more info check: http://en.cppreference.com/w/cpp/string/basic_string/getline
TEST_CASE("Client - loadLoginCredentials - automatic istream demonstration") {
    cout << "Client - loadLoginCredentials - automatic istream demonstration" << endl;

    istringstream in;

    string nicknameControl = "random nickname";
    string passwordControl = "random password";
    string nicknameBuffer;
    string passwordBuffer;

    in.str(nicknameControl + "\n" + passwordControl + "\n"); //this is prepared fake input

    int returnValue = client.loadLoginCredentials(nicknameBuffer, passwordBuffer, in);
    REQUIRE(returnValue == lStatus::SUCCESS);
    REQUIRE(nicknameControl == nicknameBuffer);
    REQUIRE(passwordControl == passwordBuffer);

    cout << "\n\n" << endl;
}

TEST_CASE("Client - LoadRegistrationCredentials - successfull registration") {
    cout << "Client - LoadRegistrationCredentials - successfull registration" << endl;

    istringstream in;

    string nicknameControl = "random nickname";
    string passwordControl = "random password";
    string nicknameBuffer;
    string passwordBuffer;
    string badPassword = "bad password";
    string password8charslong = "88888888";

    //Test of same password after 1th try
    in.str(nicknameControl + "\n" + passwordControl + "\n" + passwordControl + "\n");
    int returnValue = client.loadRegistrationCredentials(nicknameBuffer, passwordBuffer, in);

    REQUIRE(returnValue == rStatus::SUCCESS);
    REQUIRE(nicknameControl == nicknameBuffer);
    REQUIRE(passwordControl == passwordBuffer);

    nicknameBuffer.clear();
    passwordBuffer.clear();

    //Test of same password on last try
    in.str(nicknameControl + "\n" + passwordControl + "\n" + badPassword + "\n"
           + passwordControl + "\n" + badPassword + "\n"
           + passwordControl + "\n" + badPassword + "\n"
           + passwordControl + "\n" + badPassword + "\n"
           + passwordControl + "\n" + passwordControl + "\n");

    returnValue = client.loadRegistrationCredentials(nicknameBuffer, passwordBuffer, in);
    REQUIRE(returnValue == rStatus::SUCCESS);
    REQUIRE(nicknameControl == nicknameBuffer);
    REQUIRE(passwordControl == passwordBuffer);

    nicknameBuffer.clear();
    passwordBuffer.clear();

    //Test of same password with minimum length 8 chars
    in.str(nicknameControl + "\n" + password8charslong + "\n" + password8charslong + "\n");
    returnValue = client.loadRegistrationCredentials(nicknameBuffer, passwordBuffer, in);

    REQUIRE(returnValue == rStatus::SUCCESS);
    REQUIRE(nicknameControl == nicknameBuffer);
    REQUIRE(password8charslong == passwordBuffer);

    cout << "\n\n" << endl;
}

TEST_CASE("Client - LoadRegistrationCredentials - failed registration") {
    cout << "Client - LoadRegistrationCredentials - failed registration" << endl;

    istringstream in;

    string nicknameControl = "random nickname";
    string passwordControl = "random password";
    string badPassword = "bad password";
    string password7charslong = "777|777";
    string nicknameBuffer;
    string passwordBuffer;

    //Test of different passwords
    in.str(nicknameControl + "\n" + passwordControl + "\n" + badPassword + "\n"
           + passwordControl + "\n" + badPassword + "\n"
           + passwordControl + "\n" + badPassword + "\n"
           + passwordControl + "\n" + badPassword + "\n"
           + passwordControl + "\n" + badPassword + "\n");

    int returnValue = client.loadRegistrationCredentials(nicknameBuffer, passwordBuffer, in);
    REQUIRE(returnValue == rStatus::BAD_PASSWORD);
    REQUIRE(nicknameControl != nicknameBuffer);
    REQUIRE(nicknameBuffer.empty());
    REQUIRE(passwordControl != passwordBuffer);
    REQUIRE(passwordBuffer.empty());

    nicknameBuffer.clear();
    passwordBuffer.clear();

    //Test of too short password
    in.str(password7charslong + "\n" +
           password7charslong + "\n" +
           password7charslong + "\n" +
           password7charslong + "\n" +
           password7charslong + "\n");

    returnValue = client.loadRegistrationCredentials(nicknameBuffer, passwordBuffer, in);
    REQUIRE(returnValue == rStatus::BAD_PASSWORD);
    REQUIRE(nicknameBuffer.empty());
    REQUIRE(passwordBuffer.empty());

    cout << "\n\n" << endl;
}

/*
TEST_CASE("Client - Login - success"){
	cout << "Client - Login - success" << endl;

	istringstream in;

	string nicknameControl = "random nickname";
	string passwordControl = "random password";
	string nicknameBuffer;
	string passwordBuffer;

	in.str(nicknameControl + "\n" + passwordControl + "\n");

	Client client("localhost", 12345);
	int returnValue = client.login(in);
	REQUIRE(returnValue == lStatus::SUCCESS);
	REQUIRE(client.getUser().getNickname() == nicknameControl);

	REQUIRE(client.getUser().getUsedHashAlgorithm() == QCryptographicHash::Sha3_512);
	QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(passwordControl), client.getUser().getUsedHashAlgorithm());
	REQUIRE(client.getUser().getPasswordHash().toStdString() == hashed_password.toStdString());

	cout << "\n\n" << endl;
}
 */


/*
TEST_CASE("Client - Registration - success"){
	cout << "Client - Registration - success" << endl;

	istringstream in;

	string nicknameControl = "random nickname";
	string passwordControl = "random password";
	string nicknameBuffer;
	string passwordBuffer;

	in.str(nicknameControl + "\n" + passwordControl + "\n" + passwordControl + "\n");

	Client client("localhost", 12345);
	int returnValue = client.registration(in);
	REQUIRE(returnValue == rStatus::SUCCESS);
	REQUIRE(client.getUser().getNickname() == nicknameControl);

	REQUIRE(client.getUser().getUsedHashAlgorithm() == QCryptographicHash::Sha3_512);
	QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(passwordControl), client.getUser().getUsedHashAlgorithm());
	REQUIRE(client.getUser().getPasswordHash().toStdString() == hashed_password.toStdString());

	cout << "\n\n" << endl;
}
 */

/*
TEST_CASE("Client - jsonMessageUserAuthentication - Login version - JSON decoding demonstration"){
	cout << "Client - Registration - success" << endl;

	istringstream in;
	string nicknameControl = "random nickname";
	string passwordControl = "random password";
	string nicknameBuffer;
	string passwordBuffer;
	in.str(nicknameControl + "\n" + passwordControl + "\n" + passwordControl + "\n");
	Client client("localhost", 12345);
	client.login(in);

	QJsonDocument jsonMessage = client.jsonMessageUserAuthentication(MessageType::LOGIN_REQUEST);
	QByteArray byteArray = jsonMessage.toJson();



	cout << "\n\n" << endl;
}
 */


TEST_CASE("Client - quint16 conversion test") {
    QString correctPort1 = "0";
    QString correctPort2 = "1";
    QString correctPort3 = "65535"; //last possible m_port number = (2^16) - 1
    QString incorrectPort = "65536";

    quint16 testPort;
    bool returnValue = false;

    testPort = correctPort1.toUShort(&returnValue);
    REQUIRE(0 == testPort);
    REQUIRE(returnValue);

    testPort = correctPort2.toUShort(&returnValue);
    REQUIRE(1 == testPort);
    REQUIRE(returnValue);

    testPort = correctPort3.toUShort(&returnValue);
    REQUIRE(65535 == testPort);
    REQUIRE(returnValue);

    testPort = incorrectPort.toUShort(&returnValue);
    REQUIRE(!returnValue);
}
