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

TEST_CASE("json-lib fails if subscript is not natural number") {
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

TEST_CASE("json-lib performs example queries") {
  std::string json = R"({"a": { "b": [ 1, 2, { "c": "test" }, [11, 12] ]}})";

  SECTION("Trivial expressions with JSON paths") {
    REQUIRE(eval_json_query(json, "a.b[1]") == R"(2)");
    REQUIRE(eval_json_query(json, "a.b[2].c") == R"("test")");
    REQUIRE(eval_json_query(json, "a.b") ==
            R"([ 1, 2, { c: "test" }, [ 11, 12 ] ])");
  }

  SECTION("Expressions in the subscript operator []") {
    REQUIRE(eval_json_query(json, "a.b[a.b[1]].c") == R"("test")");
  }

  SECTION("Intrinsic functions: min, max, size") {
    REQUIRE(eval_json_query(json, "max(a.b[0], a.b[1])") == R"(2)");
    REQUIRE(eval_json_query(json, "min(a.b[3])") == R"(11)");

    REQUIRE(eval_json_query(json, "size(a)") == R"(1)");
    REQUIRE(eval_json_query(json, "size(a.b)") == R"(4)");
    REQUIRE(eval_json_query(json, "size(a.b[a.b[1]].c)") == R"(4)");
  }

  SECTION("Number literals") {
    REQUIRE(eval_json_query(json, "max(a.b[0], 10, a.b[1], 15)") == R"(15)");
  }
}