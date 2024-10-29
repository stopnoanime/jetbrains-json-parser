#include "json-lib.h"
#include <catch2/catch_all.hpp>

TEST_CASE("Json Lexer lexes valid json") {
  std::string json = R"({"key": [0, null] })";
  std::vector<json_lexer::Token> tokens = json_lexer::lex(json);

  REQUIRE(tokens.size() == 9);
  REQUIRE(tokens[0].type == json_lexer::OBJ_START);

  REQUIRE(tokens[1].type == json_lexer::STRING);
  REQUIRE(tokens[1].value == "key");

  REQUIRE(tokens[2].type == json_lexer::COLON);
  REQUIRE(tokens[3].type == json_lexer::ARRAY_START);

  REQUIRE(tokens[4].type == json_lexer::NUMBER);
  REQUIRE(tokens[4].value == "0");

  REQUIRE(tokens[5].type == json_lexer::COMMA);

  REQUIRE(tokens[6].type == json_lexer::CONSTANT);
  REQUIRE(tokens[6].value == "null");

  REQUIRE(tokens[7].type == json_lexer::ARRAY_END);
  REQUIRE(tokens[8].type == json_lexer::OBJ_END);
}

TEST_CASE("Json Lexer fails on invalid string") {
  REQUIRE_THROWS(json_lexer::lex("{ \"key\": \"value }"));
}

TEST_CASE("Json Lexer fails on invalid constant") {
  REQUIRE_THROWS(json_lexer::lex("{ \"key\": nu}"));
}

TEST_CASE("Json Lexer fails on invalid character") {
  REQUIRE_THROWS(json_lexer::lex("{ \"key\": 0, & }"));
}