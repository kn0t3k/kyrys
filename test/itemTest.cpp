#include "catch.hpp"

#include <reference.hpp>
#include <item.hpp>
#include <enums.hpp>
#include <limits>
#include <chat.hpp>
#include <friend.hpp>

using Kyrys::Item;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::JsonMessage::MessageType;
using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Chat::DataEncryption;
using Kyrys::Enums::Chat::Accessibility ;
using Kyrys::Chat;
using Kyrys::Friend;

#define ENABLE_JSON_EXAMPLES 1

TEST_CASE("Item constructor all valid") {
    QString json = "{\"messageType\" : \"REGISTER_REQUEST\", \"method\": \"register\",\"args\": {\"nickname\": \"Jak\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.getNick() == "Jak");
    REQUIRE(item.getMethodType() == MethodType::REGISTER);
}

TEST_CASE("Item constructor - invalid method - getter test") {
    QString json = "{\"messageType\" : \"REGISTER_REQUEST\", \"method\": \"foo\",\"args\": {\"nickname\": \"Jak\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.getMethodType() == MethodType::UNKNOWN);
}

TEST_CASE("Item constructor - invalid command - getter test") {
    QString json = "{\"messageType\" : \"REGISTER_REQUEST\", \"foo\": \"register\",\"args\": {\"nickname\": \"Jak\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.getMethodType() == MethodType::INVALID_CMND);
}

TEST_CASE("Item constructor - invalid command 2 - getter test") {
    QString json = "{\"messageType\" : \"REGISTER_REQUEST\", \"method\": \"register\",\"foo\": {\"nickname\": \"Jak\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.getMethodType() == MethodType::INVALID_CMND);
}

TEST_CASE("Item constructor - invalid command 4 - getter test") {
    QString json = "{\"messageType\" : \"REGISTER_REQUEST\", \"method\": \"register\",\"args\": {\"nickname\": \"Jak\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.getNick() == "Jak");
}

TEST_CASE("Item - IsValid - incomplete") {
    QString json = "{\"messageType\" : \"REGISTER_REQUEST\", \"method\": \"register\",\"args\": {\"nickname\": \"Jak\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.isValid() != Status::SUCCESS);
}

TEST_CASE("Item - IsValid - args") {
    QString json = "{\"messageType\" : \"REGISTER_REQUEST\", \"method\": \"register\",\"arg\": {\"nickname\": \"Jak\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.isValid() != Status::SUCCESS);
}

TEST_CASE("Item - IsValid - invalid command") {
    QString json = "{\"messageType\" : \"REGISTER_REQUEST\", \"meth\": \"register\",\"args\": {\"nickname\": \"Jak\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.isValid() != Status::SUCCESS);
}

TEST_CASE("Item - IsValid - method unknown") {
    QString json = "{\"messageType\" : \"REGISTER_REQUEST\", \"method\": \"reg\",\"args\": {\"nickname\": \"Jak\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.isValid() != Status::SUCCESS);
}

TEST_CASE("Item - IsValid - invalid json syntax") {
    QString json = "{\"messageType\" : \"REGISTER_REQUEST\", \"method\": \"reg\",\"args\" ,\"nickname\": \"Jak\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.isValid() != Status::SUCCESS);
}

TEST_CASE("Item - login - all valid") {
    QString json = "{\"messageType\" : \"LOGIN_REQUEST\", \"method\": \"login\",\"args\": {\"nickname\": \"Jak\", \"password\" : \"nejakeheslo\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.isValid() == Status::SUCCESS);
}

TEST_CASE("Item - login - invalid method") {
    QString json = "{\"messageType\" : \"LOGIN_REQUEST\",\"method\": \"log\",\"args\": {\"nickname\": \"Jak\", \"password\" : \"nejakeheslo\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.isValid() == Status::UNKNOWN_METHOD);
}

TEST_CASE("Item - login - unknown command") {
    QString json = "{\"messageType\" : \"LOGIN_REQUEST\", \"meth\": \"login\",\"args\": {\"nickname\": \"Jak\", \"password\" : \"nejakeheslo\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.isValid() == Status::INVALID_CMND);
}

TEST_CASE("Item - login - invalid json") {
    QString json = "{\"messageType\" : \"LOGIN_REQUEST\",\"method\": \"login\" \"args\": {\"nickname\": \"Jak\", \"password\" : \"nejakeheslo\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.isValid() == Status::INVALID_CMND);
}


TEST_CASE("Item - login - invalid args 1") {
    QString json = "{\"messageType\" : \"LOGIN_REQUEST\",\"method\": \"login\", \"arg\": {\"nickname\": \"Jak\", \"password\" : \"nejakeheslo\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.isValid() == Status::INVALID_CMND);
}


TEST_CASE("Item - login - invalid args 2") {
    QString json = "{\"messageType\" : \"LOGIN_REQUEST\", \"method\": \"login\", \"args\": {\"nic\": \"Jak\", \"password\" : \"nejakeheslo\"}}";
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject set = d.object();

    Item item;
    item.parse(set);

    REQUIRE(item.isValid() == Status::INVALID_CMND);
}

TEST_CASE("Item - parse & valid - valid REGISTER_REQUEST"){
    QString messageType = "REGISTER_REQUEST";
	QString method = "register";
	QString nickname = "string type";
	QString password = "string type";

	MethodType validMethodType = MethodType::REGISTER;
	MessageType validMessageType = MessageType::REGISTER_REQUEST;

	QString validMessage = "{"
								   	"\"messageType\" : \"" 	+ messageType 	+ "\", "
						   		   	"\"method\" : \"" 		+ method 		+ "\", "
						   			"\"args\" : {"
										"\"nickname\" : \"" + nickname 		+ "\", "
									   	"\"password\" : \"" + password 		+ "\""
								    "}"
							"}";
	QJsonDocument jsonDocument = QJsonDocument::fromJson(validMessage.toLocal8Bit());
	QJsonObject	jsonObject = jsonDocument.object();

	Item item;
	item.parse(jsonObject);

	REQUIRE(item.getMethodType() == validMethodType);
	REQUIRE(item.getMessageType() == validMessageType);
	REQUIRE(item.getNick() == nickname);
	REQUIRE(item.getPasswordHash() == password);

	REQUIRE(item.isValid() == Status::SUCCESS);
}

TEST_CASE("Item - parse & valid - valid REGISTER_RESPONSE"){
	QString messageType = "REGISTER_RESPONSE";
	QString method = "register";
	QString nickname = "string type";
	QString ID = "string type";
	QString success = "1";

	MethodType validMethodType = MethodType::REGISTER;
	MessageType validMessageType = MessageType::REGISTER_RESPONSE;

	QString validMessage = "{"
								   "\"messageType\" : \"" 	+ messageType 	+ "\", "
								   "\"method\" : \"" 	  	+ method 		+ "\", "
								   "\"args\" : {"
								   		"\"nickname\" : \"" + nickname 		+ "\", "
								   		"\"ID\" : \"" 		+ ID 			+ "\", "
								   		"\"success\" : \"" 	+ success 		+ "\""
								   "}"
							"}";

	QJsonDocument jsonDocument = QJsonDocument::fromJson(validMessage.toLocal8Bit());
	QJsonObject	jsonObject = jsonDocument.object();

	Item item;
	item.parse(jsonObject);

	REQUIRE(item.getMessageType() == validMessageType);
	REQUIRE(item.getMethodType() == validMethodType);
	REQUIRE(item.getNick() == nickname);
	REQUIRE(item.getID() != std::numeric_limits<unsigned int>::max());
	//REQUIRE(item.getSuccess());

	REQUIRE(item.isValid() == Status::SUCCESS);
}


TEST_CASE("Item - parse & valid - valid CHAT_DATA"){
	if(ENABLE_JSON_EXAMPLES)std::cout << "\nItem - parse & valid - valid CHAT_DATA\n" << std::endl;
	Friend sender(13, "Alice");
	Friend receiver(17, "Bob");
	QString message = "Hello World, this is testing message";
	Chat chat(sender, receiver);

	//Creating JSON CHAT_DATA message
	QJsonDocument jsonDocument = chat.jsonCreateChatData(sender, receiver, message);
	QJsonObject jsonObject = jsonDocument.object();

	//Printing example of valid CHAT_DATA message in output of test
	if(ENABLE_JSON_EXAMPLES) {
		std::string messageSTD = jsonDocument.toJson().toStdString();
		std::cout << "EXAMPLE OF VALID CHAT_DATA MESSAGE: \n" << messageSTD << std::endl;
	}

	//Parsing
	Item item;
	item.parse(jsonObject);

	//Controlling if messageType and method was parsed right
	REQUIRE(MethodType::CHAT == item.getMethodType());
	REQUIRE(MessageType::CHAT_DATA == item.getMessageType());

	//Controlling right parsing of message's args
	REQUIRE(13 == item.getFromID());
	REQUIRE(17 == item.getToID());
	REQUIRE(message == item.getData());

	//Validating of received valid JSON CHAT_DATA message
	REQUIRE(item.isValid());
}

/*
TEST_CASE("Item - parse & valid - valid CHAT_REQUEST"){
	QString messageType = "CHAT_REQUEST";
	QString method = "chat";
	QString fromID = "0";
	QString toID = "1";
	QString toNick = "some nick";
	QString dataEncryption = "0";

	MethodType validMethodType = MethodType::CHAT;
	MessageType validMessageType = MessageType::CHAT_REQUEST;
	DataEncryption validDataEncryption = DataEncryption::PLAIN_TEXT;

	//Test of plain_text encryption
	QString validMessage = "{"
								   "\"messageType\":\"" 			+ messageType 		+ "\","
								   "\"method\":\"" 	  				+ method 			+ "\","
								   "\"args\":{"
								   		"\"fromID\":"	 			+ fromID 			+ ","
								   		"\"toID\":"   				+ toID 				+ ","
 								   		"\"toNick\":\"" 			+ toNick 			+ "\","
 								   		"\"dataEncryption\":" 		+ dataEncryption 	+
								   "}"
						   "}";
	QJsonDocument jsonDocument = QJsonDocument::fromJson(validMessage.toLocal8Bit());
	QJsonObject	jsonObject = jsonDocument.object();

	Item item;
	item.parse(jsonObject);

	REQUIRE(item.getMethodType() == validMethodType);
	REQUIRE(item.getMessageType() == validMessageType);
	REQUIRE(item.getFromID() == fromID.toUInt());
	REQUIRE(item.getToID() == toID.toUInt());
	REQUIRE(item.getToNick() == toNick);
	REQUIRE(item.getEncryption() == validDataEncryption);

	REQUIRE(item.isValid() == Status::SUCCESS);

	// This part of test failing on wrong m_dataEncryption

	//Test of shared_key encryption
	dataEncryption = "1";
	validDataEncryption = DataEncryption::SHARED_KEY;

	validMessage = "{"
						   "\"messageType\":\"" 			+ messageType 		+ "\","
						   "\"method\":\"" 	  				+ method 			+ "\","
						   "\"args\":{"
						   		"\"fromID\":" 				+ fromID 			+ ","
						   		"\"toID\":"   				+ toID 				+ ","
						   		"\"toNick\": \"" 			+ toNick 			+ "\","
						   		"\"dataEncryption\":" 		+ dataEncryption 	+
						   "}"
	    			"}";

	item.clear();
	item.parse(jsonObject);

	REQUIRE(item.getMethodType() == validMethodType);
	REQUIRE(item.getMessageType() == validMessageType);
	REQUIRE(item.getFromID() == fromID.toUInt());
	REQUIRE(item.getToID() == toID.toUInt());
	REQUIRE(item.getToNick() == toNick);
	REQUIRE(item.getEncryption() == validDataEncryption);

	REQUIRE(item.isValid() == Status::SUCCESS);

}


TEST_CASE("Item - parse & valid - valid CHAT_RESPONSE"){
	QString messageType = "CHAT_RESPONSE";
	QString method = "chat";
	QString fromID = "0";
	QString toID = "1";
	QString accessibility = "0";
	QString answer = "true"; //How to insert bool type into JSON document?

	MethodType validMethodType = MethodType::CHAT;
	MessageType validMessageType = MessageType::CHAT_RESPONSE;
	Accessibility validAccess = Accessibility::ONLINE;

	QString validMessage = "{"
								   "\"messageType\": \"" 			+ messageType 		+ "\", "
								   "\"method\": \"" 	  			+ method 			+ "\", "
								   "\"args\": {"
								   		"\"fromID\": \"" 			+ fromID 			+ "\", "
								   		"\"toID\": \""   			+ toID 				+ "\", "
								   		"\"accessibility\": \"" 	+ accessibility		+ "\", "
								   		"\"answer\": "	 			+ answer		 	+
								   "}"
						   "}";

	QJsonDocument jsonDocument = QJsonDocument::fromJson(validMessage.toLocal8Bit());
	QJsonObject	jsonObject = jsonDocument.object();

	Item item;
	item.parse(jsonObject);

	QJsonObject args = jsonObject["args"].toObject();

	REQUIRE(item.getMethodType() == validMethodType);
	REQUIRE(item.getMessageType() == validMessageType);
	REQUIRE(item.getFromID() == fromID.toUInt());
	REQUIRE(item.getToID() == toID.toUInt());
	REQUIRE(item.getAccessibility() == validAccess);

	//REQUIRE(args["answer"].isBool());	//How to insert bool type into JSON document?
	REQUIRE(item.getAnswer());
	REQUIRE(item.isValid() == Status::SUCCESS);
}


TEST_CASE("Item - parse & valid - valid CHAT_DATA"){
	QString messageType = "CHAT_DATA";
	QString method = "chat";
	QString fromID = "0";
	QString toID = "1";
	QString data = "Hello user 1, I would like to start chat with you";

	MethodType validMethodType = MethodType::CHAT;
	MessageType validMessageType = MessageType::CHAT_DATA;

	QString validMessage = "{"
								   "\"messageType\": \"" 	+ messageType 	+ "\", "
								   "\"method\": \"" 	  	+ method 		+ "\", "
								   "\"args\": {"
								   		"\"fromID\": \"" 	+ fromID 		+ "\", "
								   		"\"toID\": \""   	+ toID 			+ "\", "
								   		"\"data\": \"" 		+ data			+ "\", "
								   "}"
							"}";

	QString validMessage2 = "{\"messageType\": \"" + messageType +
							"\",\"method\": \"" + method +
							"\",\"args\": {\"fromID\": \"" + fromID +
							"\",\"toID\": \"" + toID +
							"\",\"data\": \"" + data + "\"}}";

	QJsonDocument jsonDocument = QJsonDocument::fromJson(validMessage.toLocal8Bit());
	QJsonObject	jsonObject = jsonDocument.object();

	Item item;
	item.parse(jsonObject);

	QJsonObject args = jsonObject["args"].toObject();

	REQUIRE(item.getMethodType() == validMethodType);
	REQUIRE(item.getMessageType() == validMessageType);
	REQUIRE(item.getFromID() == fromID.toUInt());
	REQUIRE(item.getToID() == toID.toUInt());
	REQUIRE(item.getData() == data);

	REQUIRE(item.isValid() == Status::SUCCESS);
}
*/
