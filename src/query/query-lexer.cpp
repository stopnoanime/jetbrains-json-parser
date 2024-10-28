#include "query-lexer.h"

namespace query_lexer {
std::vector<Token> lex(std::string &str) {
  std::stringstream ss(str);
  std::vector<Token> tokens;
  char c;

  while (ss >> c) {
    std::string value;

    switch (c) {
    case '.':
      tokens.push_back({DOT, value});
      break;

    case '[':
      tokens.push_back({SUBSCRIPT_START, value});
      break;

    case ']':
      tokens.push_back({SUBSCRIPT_END, value});
      break;

    case '(':
      tokens.push_back({ARGS_START, value});
      break;

    case ',':
      tokens.push_back({COMMA, value});
      break;

    case ')':
      tokens.push_back({ARGS_END, value});
      break;

    default:
      if (std::isspace(c))
        break;

      if (std::isdigit(c)) {
        value += c;

        while (std::isdigit(ss.peek()))
          value += ss.get();

        tokens.push_back({NUMBER, value});

        break;
      }

      if (std::isalnum(c)) {
        value += c;

        while (std::isalnum(ss.peek()))
          value += ss.get();

        tokens.push_back({IDENTIFIER, value});

        break;
      }

      throw std::runtime_error("Invalid Query format.");
    }
  }

  return tokens;
}
} // namespace query_lexer