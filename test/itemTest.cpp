#include "catch.hpp"

#include <reference.hpp>
#include <item.hpp>
#include <enums.hpp>

using Kyrys::Item;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Status;

TEST_CASE("Item constructor all valid") {
	QString json = "{\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}";
	QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject set = d.object();

	Item item(set);

	REQUIRE(item.Name() == "Jan Novak");
	REQUIRE(item.Nick() == "Jak");
	REQUIRE(item.Method() == MethodType::REGISTER);
}

TEST_CASE("Item constructor - invalid method - getter test") {
	QString json = "{\"method\": \"foo\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}";
	QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject set = d.object();

	Item item(set);

	REQUIRE(item.Name() == "Jan Novak");
	REQUIRE(item.Nick() == "Jak");
	REQUIRE(item.Method() == MethodType::UNKNOWN);
}

TEST_CASE("Item constructor - invalid command - getter test") {
	QString json = "{\"foo\": \"register\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}";
	QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject set = d.object();

	Item item(set);

	REQUIRE(item.Name() == "Jan Novak");
	REQUIRE(item.Nick() == "Jak");
	REQUIRE(item.Method() == MethodType::INVALID_CMND);
}

TEST_CASE("Item constructor - invalid command 2 - getter test") {
	QString json = "{\"method\": \"register\",\"foo\": {\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}";
	QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject set = d.object();

	Item item(set);

	REQUIRE(item.Method() == MethodType::INVALID_CMND);
}

TEST_CASE("Item constructor - invalid command 3 - getter test") {
	QString json = "{\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\"}}";
	QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject set = d.object();

	Item item(set);

	REQUIRE(item.Name() == "Jan Novak");
}

TEST_CASE("Item constructor - invalid command 4 - getter test") {
	QString json = "{\"method\": \"register\",\"args\": {\"nick\": \"Jak\"}}";
	QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject set = d.object();

	Item item(set);

	REQUIRE(item.Nick() == "Jak");
	REQUIRE(item.Name() == "");
}

TEST_CASE("Item - IsValid - incomplete") {
	QString json = "{\"method\": \"register\",\"args\": {\"nick\": \"Jak\"}}";
	QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject set = d.object();

	Item item(set);

	REQUIRE(item.IsValid() != Status::SUCCESS);
}

TEST_CASE("Item - IsValid - args") {
	QString json = "{\"method\": \"register\",\"arg\": {\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}";
	QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject set = d.object();

	Item item(set);

	REQUIRE(item.IsValid() != Status::SUCCESS);
}

TEST_CASE("Item - IsValid - invalid command") {
	QString json = "{\"meth\": \"register\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}";
	QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject set = d.object();

	Item item(set);

	REQUIRE(item.IsValid() != Status::SUCCESS);
}

TEST_CASE("Item - IsValid - method unknown") {
	QString json = "{\"method\": \"reg\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}";
	QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject set = d.object();

	Item item(set);

	REQUIRE(item.IsValid() != Status::SUCCESS);
}

TEST_CASE("Item - IsValid - invalid json syntax") {
	QString json = "{\"method\": \"reg\",\"args\" \"name\": \"Jan Novak\",\"nick\": \"Jak\"}}";
	QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject set = d.object();

	Item item(set);

	REQUIRE(item.IsValid() != Status::SUCCESS);
}
