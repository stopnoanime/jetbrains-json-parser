#ifndef QUERY_LEXER_H
#define QUERY_LEXER_H

#include <sstream>
#include <string>
#include <vector>

namespace query_lexer {
  enum TokenType {
    IDENTIFIER,
    NUMBER,
    DOT,
    SUBSCRIPT_START,
    SUBSCRIPT_END,
    ARGS_START,
    COMMA,
    ARGS_END
  };

  struct Token {
    TokenType type;
    std::string value;
  };

  std::vector<Token> lex(std::string &str);
};

#endif
