#include "json-lexer.h"

namespace json_lexer {

std::vector<Token> lex(std::string &str) {
  std::stringstream ss(str);
  std::vector<Token> tokens;
  char c;

  while (ss >> c) {
    std::string value;

    switch (c) {
    case '{':
      tokens.push_back({OBJ_START, value});
      break;

    case '}':
      tokens.push_back({OBJ_END, value});
      break;

    case '[':
      tokens.push_back({ARRAY_START, value});
      break;

    case ']':
      tokens.push_back({ARRAY_END, value});
      break;

    case ':':
      tokens.push_back({COLON, value});
      break;

    case ',':
      tokens.push_back({COMMA, value});
      break;

    case '"':
      std::getline(ss, value, '"');
      tokens.push_back({STRING, value});
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

      std::string next4(4, c);
      ss.read(&next4[1], 3);

      if (next4 == "true" || next4 == "false" || next4 == "null")
        tokens.push_back({CONSTANT, next4});
      else
        throw std::runtime_error("Invalid JSON format.");
    }
  }

  return tokens;
}

} // namespace json_lexer
