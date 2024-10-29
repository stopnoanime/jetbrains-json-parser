#include "json-lib.h"
#include <catch2/catch_all.hpp>

TEST_CASE("json-lib parses json and evaluates queries") {
  std::string json = R"({"key": ["value", 2]})";

  SECTION("empty query returns entire object") {
    REQUIRE(eval_json_query(json, "") == R"({ key: [ "value", 2 ] })");
  }

  SECTION("basic query for object access") {
    REQUIRE(eval_json_query(json, "key") == R"([ "value", 2 ])");
  }

  SECTION("basic subscript query for array access") {
    REQUIRE(eval_json_query(json, "key[1]") == "2");
  }

  SECTION("allows for functions in query") {
    REQUIRE(eval_json_query(json, "min(key[1], -1)") == "-1");
  }
}

TEST_CASE("json-lib fails if root is not object") {
  REQUIRE(eval_json_query("[1, 2]", "").substr(0, 5) == "Error");
}

TEST_CASE("json-lib fails if subscript is not natural") {
  REQUIRE(eval_json_query(R"({"a" : [1, 2]})", "a[1.2]").substr(0, 5) ==
          "Error");
}

TEST_CASE("json-lib displays error on invalid json") {
  REQUIRE(eval_json_query("{", "").substr(0, 5) == "Error");
}

TEST_CASE("json-lib displays error on invalid query") {
  REQUIRE(eval_json_query("{}", "key").substr(0, 5) == "Error");
}

TEST_CASE("json-lib displays error on invalid query function") {
  REQUIRE(eval_json_query("{}", "mi(1)").substr(0, 5) == "Error");
  REQUIRE(eval_json_query("{}", "min(1, )").substr(0, 5) == "Error");
}