#include "json-lib.h"
#include <catch2/catch_all.hpp>
#include <memory>

TEST_CASE("Eval evaluates empty query") {
  std::vector<query_lexer::Token> tokens;
  std::map<std::string, std::unique_ptr<json::Node>> values;
  json::Object root(std::move(values));

  REQUIRE(query_eval::eval(root, tokens) == "{  }");
}

TEST_CASE("Eval evaluates basic query") {
  std::vector<query_lexer::Token> tokens = {
      {query_lexer::IDENTIFIER, "a"},
  };

  std::map<std::string, std::unique_ptr<json::Node>> values;
  values["a"] = std::make_unique<json::Number>(1);
  json::Object object(std::move(values));

  REQUIRE(query_eval::eval(object, tokens) == "1");
}

TEST_CASE("Eval fails if root node is not an object") {
  json::Node root = json::Number(0);
  std::vector<query_lexer::Token> tokens;

  REQUIRE_THROWS(query_eval::eval(root, tokens));
}

TEST_CASE("Eval fails if it can't evaluate the entire query") {
  std::vector<query_lexer::Token> tokens = {
      {query_lexer::IDENTIFIER, "a"},
      {query_lexer::IDENTIFIER, "b"},
  };

  std::map<std::string, std::unique_ptr<json::Node>> values;
  values["a"] = std::make_unique<json::Number>(1);
  json::Object object(std::move(values));

  REQUIRE_THROWS(query_eval::eval(object, tokens));
}