#include "lexer.h"

std::vector<Token> lexer(std::string &str) {
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
      if(std::isspace(c))
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

      if (next4 == "true")
        tokens.push_back({VAL_TRUE, value});
      else if (next4 == "false")
        tokens.push_back({VAL_FALSE, value});
      else if (next4 == "null")
        tokens.push_back({VAL_NULL, value});
      else
        throw std::runtime_error("Invalid JSON format.");
    }
  }

  return tokens;
}