#pragma once

#include <string>
#include <vector>

namespace json_lexer {

enum TokenType {
  OBJ_START,
  OBJ_END,
  ARRAY_START,
  ARRAY_END,
  COLON,
  COMMA,
  STRING,
  NUMBER,
  CONSTANT
};

struct Token {
  TokenType type;
  std::string value;
};

std::vector<Token> lex(const std::string &str);

} // namespace json_lexer
