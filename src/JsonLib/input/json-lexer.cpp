#include "json-lexer.h"
#include <format>
#include <sstream>
#include <stdexcept>

namespace json_lexer {

Token read_token(char c, std::stringstream &ss) {
  std::string value;

  switch (c) {
  case '{':
    return {OBJ_START, value};
    break;

  case '}':
    return {OBJ_END, value};
    break;

  case '[':
    return {ARRAY_START, value};
    break;

  case ']':
    return {ARRAY_END, value};
    break;

  case ':':
    return {COLON, value};
    break;

  case ',':
    return {COMMA, value};
    break;

  case '"':
    std::getline(ss, value, '"');
    if (ss.eof() || ss.fail())
      throw std::runtime_error("Could not find string's closing quote.");

    return {STRING, value};
    break;

  default:
    if (std::isspace(c))
      break;

    // go back one character and try reading number
    ss.seekg(-1, std::ios_base::cur);
    double num;
    if (ss >> num) {
      return {NUMBER, std::format("{}", num)};
      break;
    }

    ss.clear(); // reading number failed so we must clear the error flag

    // try reading constant
    const std::vector<std::string> constants = {"true", "false", "null"};
    std::string buffer = ss.str().substr(ss.tellg());

    for (auto &s : constants) {
      if (buffer.compare(0, s.size(), s) == 0) {
        ss.seekg(s.size(), std::ios_base::cur);
        return {CONSTANT, s};
      }
    }
  }

  throw std::runtime_error("Unexpected character found while lexing JSON.");
}

std::vector<Token> lex(const std::string &str) {
  std::stringstream ss(str);
  std::vector<Token> tokens;
  char c;

  while (ss >> c)
    tokens.push_back(read_token(c, ss));

  return tokens;
}

} // namespace json_lexer
