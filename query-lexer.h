#ifndef QUERY_LEXER_H
#define QUERY_LEXER_H

#include <sstream>
#include <string>
#include <vector>

class QueryLexer {
public:
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

  static std::vector<Token> lex(std::string &str);
};

#endif
