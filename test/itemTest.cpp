#include "catch.hpp"

#include <reference.hpp>
#include <item.hpp>
#include <enums.hpp>

using Kyrys::Item;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Status;

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





