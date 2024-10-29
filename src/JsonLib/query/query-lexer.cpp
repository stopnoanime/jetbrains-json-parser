#include "query-lexer.h"
#include <sstream>
#include <stdexcept>

namespace query_lexer {

std::vector<Token> lex(const std::string &str) {
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

      // read identifier
      if (std::isalpha(c) || c == '_') {
        value += c;

        while (std::isalnum(ss.peek()) || ss.peek() == '_')
          value += ss.get();

        tokens.push_back({IDENTIFIER, value});

        break;
      }

      // go back one character and try reading a number
      ss.seekg(-1, std::ios_base::cur);
      double num;
      if (ss >> num) {
        tokens.push_back({NUMBER, std::to_string(num)});
        break;
      }

      throw std::runtime_error(
          "Unexpected character found while lexing query.");
    }
  }

  return tokens;
}
} // namespace query_lexer