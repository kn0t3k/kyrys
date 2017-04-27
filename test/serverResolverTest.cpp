#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif

#include "catch.hpp"

#include <reference.hpp>

#include <serverResolver.hpp>
#include <enums.hpp>
#include <utils.hpp>

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Utils::Random;

Kyrys::ServerResolver r;

TEST_CASE("Resolver - valid JSON syntax") {

    QString jsonInput = "{\"messageType\": \"REGISTER_REQUEST\",\"method\": \"register\",\"args\": {\"nickname\": \"Jak1\", \"password\": \"somepass\" }}";

    int result = r.parse(jsonInput, Mode::USE_JSON);
    REQUIRE(result == Status::SUCCESS);
}

TEST_CASE("Resolver - save to file") {
    Random rnd;
    QString randStr = rnd.getRandomString(25);

    QString jsonInput = "{\"messageType\" : \"REGISTER_REQUEST\",\"method\": \"register\",\"args\": {\"password\": \"somepass\",\"nickname\": \"";
    jsonInput += randStr;
    jsonInput += "\"}}";

    int result = r.parse(jsonInput, Mode::USE_JSON);

    REQUIRE(result == Status::SUCCESS);

    QString databasePath = QString(DATABASE_DIRECTORY) + "/" + QString(DATABASE_FILENAME);

    if (!QDir(DATABASE_DIRECTORY).exists()) {
        QDir().mkdir(DATABASE_DIRECTORY);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);
    if (db.open()) {
        qDebug() << "db opened successfully";
        QSqlQuery query;
        query.prepare("SELECT ids FROM users WHERE nickname = (:nickname)");
        query.bindValue(":nickname", randStr);
        if (query.exec()) {
            REQUIRE(query.next());
        } else {
            qDebug() << "error with select";
        }
    } else {
        qDebug() << "db failed to open at " + databasePath;
        return;
    }
}

TEST_CASE("Resolver - invalid JSON syntax") {
    int result = r.parse(
            "\"method\": \"register\",\"args\": {\"password\": \"somepass\",\"nickname\": \"Jak2\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_JSON);
}

TEST_CASE("Resolver - incorrect command") {
    int result = r.parse(
            "{\"messageType\" : \"REGISTER_REQUEST\",\"what\": \"register\",\"args\": {\"password\": \"somepass\",\"nickname\": \"Jak3\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - unknown method") {
    int result = r.parse(
            "{\"messageType\" : \"REGISTER_REQUEST\",\"method\": \"reg\",\"args\": {\"password\": \"somepass\",\"nickname\": \"Ja3k\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::UNKNOWN_METHOD);
}

TEST_CASE("Resolver - unknown resolver mode") {
    int result = r.parse(
            "{\"messageType\" : \"REGISTER_REQUEST\",\"method\": \"reg\",\"args\": {\"password\": \"somepass\",\"nickname\": \"Ja2k\"}}",
            Mode::INVALID);
    REQUIRE(result == Status::FAILED);
}

TEST_CASE("Resolver - invalid args") {
    int result = r.parse(
            "{\"messageType\" : \"REGISTER_REQUEST\",\"method\": \"register\",\"ars\": {\"password\": \"somepass\",\"nickname\": \"Jak4\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - invalid args 2") {
    int result = r.parse(
            "{\"messageType\" : \"REGISTER_REQUEST\",\"method\": \"register\",\"args\": {\"passwrd\": \"somepass\",\"nickname\": \"Jak5\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - invalid args 3") {
    int result = r.parse(
            "{\"messageType\" : \"REGISTER_REQUEST\",\"method\": \"register\",\"args\": { \"password\": \"somepass\",\"nic\": \"Jak6\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - invalid args 4") {
    int result = r.parse(
            "{\"messageType\" : \"REGISTER_REQUEST\",\"method\": \"register\",\"args\": {\"password\": \"somepass\",\"nickname\": \"\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - login - invalid args") {
    int result = r.parse(
            "{\"messageType\" : \"LOGIN_REQUEST\",\"method\": \"login\",\"args\": {\"passord\": \"somepass\",\"nickname\": \"\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
    REQUIRE(!r.isLogin());
    REQUIRE(!r.isForward());
}

TEST_CASE("Resolver - login - invalid args 2") {
    int result = r.parse(
            "{\"messageType\" : \"LOGIN_REQUEST\",\"messageType\" : \"LOGIN_REQUEST\", \"method\": \"login\",\"args\": {\"password\": \"somepass\",\"nickname\": \"\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
    REQUIRE(!r.isLogin());
    REQUIRE(!r.isForward());
}

TEST_CASE("Resolver - login - all valid") {
    int result = r.parse(
            "{\"messageType\" : \"LOGIN_REQUEST\",\"method\": \"login\",\"args\": {\"password\": \"somepass\",\"nickname\": \"Jak1\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::SUCCESS);
    REQUIRE(r.isLogin());
    REQUIRE(!r.isForward());
}

TEST_CASE("Resolver - login - invalid nickname") {
    int result = r.parse(
            "{\"messageType\" : \"LOGIN_REQUEST\",\"method\": \"login\",\"args\": {\"password\": \"somepass\",\"nickname\": \"foo\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CRED);
    REQUIRE(!r.isLogin());
    REQUIRE(!r.isForward());
}

TEST_CASE("Resolver - login - invalid password") {
    int result = r.parse(
            "{\"messageType\" : \"LOGIN_REQUEST\",\"method\": \"login\",\"args\": {\"password\": \"foobar\",\"nickname\": \"Jak1\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CRED);
    REQUIRE(!r.isLogin());
    REQUIRE(!r.isForward());
}


TEST_CASE("Resolver - register and login") {
    QString jsonInput = "{\"messageType\" : \"REGISTER_REQUEST\", \"method\": \"register\",\"args\": {\"nickname\": \"MyUniqueNick\", \"password\": \"somepass\" }}";

    int result = r.parse(jsonInput, Mode::USE_JSON);

    REQUIRE(result == Status::SUCCESS);

    jsonInput = "{\"messageType\" : \"LOGIN_REQUEST\", \"method\": \"login\",\"args\": {\"password\": \"somepass\",\"nickname\": \"MyUniqueNick\"}}";

    result = r.parse(jsonInput, Mode::USE_JSON);

    REQUIRE(result == Status::SUCCESS);
    REQUIRE(r.isLogin());
    REQUIRE(!r.isForward());


    jsonInput = "{\"messageType\" : \"LOGIN_REQUEST\", \"method\": \"login\",\"args\": {\"password\": \"foobar\",\"nickname\": \"MyUniqueNick\"}}";
    result = r.parse(jsonInput, Mode::USE_JSON);

    REQUIRE(result == Status::INVALID_CRED);
    REQUIRE(!r.isLogin());
    REQUIRE(!r.isForward());

}


