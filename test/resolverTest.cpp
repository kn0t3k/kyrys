#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "catch.hpp"

#include <reference.hpp>

#include <resolver.hpp>
#include <enums.hpp>
#include <utils.hpp>

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Utils::Random;


TEST_CASE("Resolver - valid JSON syntax") {
	Kyrys::Resolver r("c:\\__TEMP__\\");("c:\\__TEMP__\\");
	QString jsonInput = "{\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}";

	int result = r.Parse( jsonInput, Mode::USE_JSON);
	REQUIRE(result == Status::SUCCESS);	
}

TEST_CASE("Resolver - save to file"){
	Kyrys::Resolver r("c:\\__TEMP__\\");;
	Random rnd;
	QString randStr = rnd.GetRandomString();

	QString jsonInput = "{\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"";
	jsonInput += randStr;
	jsonInput += "\"}}";

	int result = r.Parse( jsonInput, Mode::USE_JSON);

	REQUIRE(result == Status::SUCCESS);

	QString fileLoc(TEMP_DIR);
	fileLoc += "/db.txt";
	QFile file(fileLoc);

	REQUIRE(file.open(QIODevice::ReadOnly) != 0);
	// read whole file
	// check if some random stirng as nick is in it

	bool status = false;

	QTextStream stream(&file);
	QString line;
	do {
		line = stream.readLine();
		if (line.contains(randStr, Qt::CaseSensitive)) {
			status = true;
			break;
		}
	} while (!line.isNull());

	REQUIRE(status);
}

TEST_CASE("Resolver - invalid JSON syntax") {
	Kyrys::Resolver r("c:\\__TEMP__\\");;
	int result = r.Parse( "\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}", Mode::USE_JSON);
	REQUIRE(result == Status::INVALID_JSON);
}

TEST_CASE("Resolver - incorrect command") {
	Kyrys::Resolver r("c:\\__TEMP__\\");;
	int result = r.Parse( "{\"what\": \"register\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}", Mode::USE_JSON);
	REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - unknown method") {
	Kyrys::Resolver r("c:\\__TEMP__\\");;
	int result = r.Parse( "{\"method\": \"reg\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}", Mode::USE_JSON);
	REQUIRE(result == Status::UNKNOWN_METHOD);
}

TEST_CASE("Resolver - unknown resolver mode") {
	Kyrys::Resolver r("c:\\__TEMP__\\");;
	int result = r.Parse( "{\"method\": \"reg\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}", Mode::INVALID);
	REQUIRE(result == Status::FAILED);
}

TEST_CASE("Resolver - invalid args") {
	Kyrys::Resolver r("c:\\__TEMP__\\");;
	int result = r.Parse( "{\"method\": \"register\",\"ars\": {\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}", Mode::USE_JSON);
	REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - invalid args 2") {
	Kyrys::Resolver r("c:\\__TEMP__\\");;
	int result = r.Parse( "{\"method\": \"register\",\"args\": {\"nae\": \"Jan Novak\",\"nick\": \"Jak\"}}", Mode::USE_JSON);
	REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - invalid args 3") {
	Kyrys::Resolver r("c:\\__TEMP__\\");;
	int result = r.Parse( "{\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\",\"nic\": \"Jak\"}}", Mode::USE_JSON);
	REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - invalid args 4") {
	Kyrys::Resolver r("c:\\__TEMP__\\");;
	int result = r.Parse( "{\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"\"}}", Mode::USE_JSON);
	REQUIRE(result == Status::INVALID_CMND);
}
