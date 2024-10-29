#include "json-lib.h"
#include <catch2/catch_all.hpp>

TEST_CASE("Json Parser parses valid json tokens") {
  std::vector<json_lexer::Token> tokens = {
      {json_lexer::OBJ_START, "{"}, {json_lexer::STRING, "a"},
      {json_lexer::COLON, ":"},     {json_lexer::NUMBER, "10"},
      {json_lexer::OBJ_END, "}"},
  };

  auto out = json_parser::parse(tokens);
  REQUIRE(out->getType() == json::OBJECT);
  REQUIRE(static_cast<json::Object *>(out.get())->getSize() == 1);

  auto a = &static_cast<json::Object *>(out.get())->operator[]("a");
  REQUIRE(a->getType() == json::NUMBER);
  REQUIRE(static_cast<const json::Number *>(a)->getValue() == 10);
}

TEST_CASE("Json Parser parses valid json array") {
  std::vector<json_lexer::Token> tokens = {{json_lexer::ARRAY_START, "["},
                                           {json_lexer::NUMBER, "1"},
                                           {json_lexer::COMMA, ","},
                                           {json_lexer::STRING, "abcd"},
                                           {json_lexer::ARRAY_END, "]"}};

  auto out = json_parser::parse(tokens);
  REQUIRE(out->getType() == json::ARRAY);
  REQUIRE(static_cast<json::Array *>(out.get())->getSize() == 2);

  auto second = &static_cast<json::Array *>(out.get())->operator[](1);
  REQUIRE(second->getType() == json::STRING);
  REQUIRE(static_cast<const json::String *>(second)->getValue() == "abcd");
}

TEST_CASE("Json Parser fails when it can't pass all tokens") {
  std::vector<json_lexer::Token> tokens = {
      {json_lexer::OBJ_START, "{"},
      {json_lexer::OBJ_END, "}"},
      {json_lexer::OBJ_END, "}"},
  };

  REQUIRE_THROWS(json_parser::parse(tokens));
}

TEST_CASE("Json Parser fails on invalid object") {
  std::vector<json_lexer::Token> tokens = {
      {json_lexer::OBJ_START, "{"}, {json_lexer::STRING, "a"},
      {json_lexer::COLON, ":"},     {json_lexer::NUMBER, "1"},
      {json_lexer::COMMA, ","},     {json_lexer::OBJ_END, "}"},
  };

  REQUIRE_THROWS(json_parser::parse(tokens));
}

TEST_CASE("Json Parser fails on invalid array") {
  std::vector<json_lexer::Token> tokens = {
      {json_lexer::ARRAY_START, "["},
      {json_lexer::NUMBER, "1"},
      {json_lexer::COMMA, ","},
      {json_lexer::ARRAY_END, "]"},
  };

  REQUIRE_THROWS(json_parser::parse(tokens));
}