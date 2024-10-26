#ifndef LEXER_H
#define LEXER_H

#include <sstream>
#include <string>
#include <vector>

enum TokenType {
  OBJ_START,
  OBJ_END,
  ARRAY_START,
  ARRAY_END,
  COLON,
  COMMA,
  STRING,
  VAL_TRUE,
  VAL_FALSE,
  VAL_NULL,
  NUMBER
};

struct Token {
  TokenType type;
  std::string value;
};

std::vector<Token> lexer(std::string &str);

#endif
