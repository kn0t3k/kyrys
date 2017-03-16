#include <resolver.hpp>
#include <enums.hpp>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Resolver::Mode;

Kyrys::Resolver r;

TEST_CASE("Resolver Parse valid JSON") {
	int result = r.Parse( "{\"method\": \"register\",\"args\": {\"ID\": \"\",\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}", Mode::USE_JSON);
	REQUIRE(result == Status::SUCCESS);
}

TEST_CASE("Resolver Parse invalid JSON") {
	int result = r.Parse( "\"method\": \"register\",\"args\": {\"ID\": \"\",\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}", Mode::USE_JSON);
	REQUIRE(result == Status::INVALID_JSON);
}

TEST_CASE("Resolver Parse valid JSON - unknown method") {
	int result = r.Parse( "{\"what\": \"register\",\"args\": {\"ID\": \"\",\"name\": \"Jan Novak\",\"nick\": \"Jak\"}}", Mode::USE_JSON);
	REQUIRE(result == Status::UNKNOWN_METHOD);
}
