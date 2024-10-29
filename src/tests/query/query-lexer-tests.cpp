#include "json-lib.h"
#include <catch2/catch_all.hpp>

TEST_CASE("Query lexer lexes query") {
  std::string query = R"(a.b[min(-1.3, c)])";
  auto tokens = query_lexer::lex(query);
  REQUIRE(tokens.size() == 11);

  REQUIRE(tokens[0].type == query_lexer::IDENTIFIER);
  REQUIRE(tokens[0].value == "a");

  REQUIRE(tokens[1].type == query_lexer::DOT);

  REQUIRE(tokens[2].type == query_lexer::IDENTIFIER);
  REQUIRE(tokens[2].value == "b");

  REQUIRE(tokens[3].type == query_lexer::SUBSCRIPT_START);

  REQUIRE(tokens[4].type == query_lexer::IDENTIFIER);
  REQUIRE(tokens[4].value == "min");

  REQUIRE(tokens[5].type == query_lexer::ARGS_START);

  REQUIRE(tokens[6].type == query_lexer::NUMBER);
  REQUIRE(tokens[6].value == "-1.3");

  REQUIRE(tokens[7].type == query_lexer::COMMA);

  REQUIRE(tokens[8].type == query_lexer::IDENTIFIER);
  REQUIRE(tokens[8].value == "c");

  REQUIRE(tokens[9].type == query_lexer::ARGS_END);

  REQUIRE(tokens[10].type == query_lexer::SUBSCRIPT_END);
}

TEST_CASE("Query lexer fails on query with invalid characters") {
  std::string query = R"(a.b[min(1, &)])";
  REQUIRE_THROWS(query_lexer::lex(query));
}
