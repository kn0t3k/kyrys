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


TEST_CASE("Resolver - valid JSON syntax") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");
    QString jsonInput = "{\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"Jak1\", \"password\": \"somepass\" }}";

    int result = r.parse(jsonInput, Mode::USE_JSON);
    REQUIRE(result == Status::SUCCESS);
}

TEST_CASE("Resolver - save to file") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    Random rnd;
    QString randStr = rnd.getRandomString(25);

    QString jsonInput = "{\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\", \"password\": \"somepass\",\"nick\": \"";
    jsonInput += randStr;
    jsonInput += "\"}}";

    int result = r.parse(jsonInput, Mode::USE_JSON);

    REQUIRE(result == Status::SUCCESS);

    QString fileLoc("c:\\__TEMP__\\");
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
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    int result = r.parse(
            "\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\", \"password\": \"somepass\",\"nick\": \"Jak2\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_JSON);
}

TEST_CASE("Resolver - incorrect command") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    int result = r.parse(
            "{\"what\": \"register\",\"args\": {\"name\": \"Jan Novak\", \"password\": \"somepass\",\"nick\": \"Jak3\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - unknown method") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    int result = r.parse(
            "{\"method\": \"reg\",\"args\": {\"name\": \"Jan Novak\", \"password\": \"somepass\",\"nick\": \"Ja3k\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::UNKNOWN_METHOD);
}

TEST_CASE("Resolver - unknown resolver mode") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    int result = r.parse(
            "{\"method\": \"reg\",\"args\": {\"name\": \"Jan Novak\", \"password\": \"somepass\",\"nick\": \"Ja2k\"}}",
            Mode::INVALID);
    REQUIRE(result == Status::FAILED);
}

TEST_CASE("Resolver - invalid args") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    int result = r.parse(
            "{\"method\": \"register\",\"ars\": {\"name\": \"Jan Novak\", \"password\": \"somepass\",\"nick\": \"Jak4\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - invalid args 2") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    int result = r.parse(
            "{\"method\": \"register\",\"args\": {\"nae\": \"Jan Novak\", \"password\": \"somepass\",\"nick\": \"Jak5\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - invalid args 3") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    int result = r.parse(
            "{\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\", \"password\": \"somepass\",\"nic\": \"Jak6\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - invalid args 4") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    int result = r.parse(
            "{\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\", \"password\": \"somepass\",\"nick\": \"\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
}

TEST_CASE("Resolver - login - invalid args") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    int result = r.parse(
            "{\"method\": \"login\",\"args\": {\"passord\": \"somepass\",\"nick\": \"\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
    REQUIRE(!r.isLogin());
    REQUIRE(!r.isForward());
}

TEST_CASE("Resolver - login - invalid args 2") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    int result = r.parse(
            "{\"method\": \"login\",\"args\": {\"password\": \"somepass\",\"nick\": \"\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CMND);
    REQUIRE(!r.isLogin());
    REQUIRE(!r.isForward());
}

TEST_CASE("Resolver - login - all valid") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    int result = r.parse(
            "{\"method\": \"login\",\"args\": {\"password\": \"somepass\",\"nick\": \"Jak1\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::SUCCESS);
    REQUIRE(r.isLogin());
    REQUIRE(!r.isForward());
}

TEST_CASE("Resolver - login - invalid nick") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    int result = r.parse(
            "{\"method\": \"login\",\"args\": {\"password\": \"somepass\",\"nick\": \"foo\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CRED);
    REQUIRE(!r.isLogin());
    REQUIRE(!r.isForward());
}

TEST_CASE("Resolver - login - invalid password") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");;
    int result = r.parse(
            "{\"method\": \"login\",\"args\": {\"password\": \"foobar\",\"nick\": \"Jak1\"}}",
            Mode::USE_JSON);
    REQUIRE(result == Status::INVALID_CRED);
    REQUIRE(!r.isLogin());
    REQUIRE(!r.isForward());
}

TEST_CASE("Resolver - register and login") {
    Kyrys::ServerResolver r("c:\\__TEMP__\\");
    QString jsonInput = "{\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"MyUniqueNick\", \"password\": \"somepass\" }}";

    int result = r.parse(jsonInput, Mode::USE_JSON);

    REQUIRE(result == Status::SUCCESS);

    jsonInput = "{\"method\": \"login\",\"args\": {\"password\": \"somepass\",\"nick\": \"MyUniqueNick\"}}";

    result = r.parse(jsonInput, Mode::USE_JSON);

    REQUIRE(result == Status::SUCCESS);
    REQUIRE(r.isLogin());
    REQUIRE(!r.isForward());

    jsonInput = "{\"method\": \"login\",\"args\": {\"password\": \"foobar\",\"nick\": \"MyUniqueNick\"}}";
    result = r.parse(jsonInput, Mode::USE_JSON);

    REQUIRE(result == Status::INVALID_CRED);
    REQUIRE(!r.isLogin());
    REQUIRE(!r.isForward());
}

