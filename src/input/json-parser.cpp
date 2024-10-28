#include "json-parser.h"

namespace json_parser {

#define check_end()                                                            \
  if (start == end)                                                            \
    throw std::runtime_error("Unexpected end of JSON tokens.");

#define return_on_token(token)                                                 \
  check_end();                                                                 \
  if ((*start).type == token) {                                                \
    start++;                                                                   \
    return values;                                                             \
  }

std::vector<std::unique_ptr<json::Node>> parse_array(iter &start, iter &end) {
  std::vector<std::unique_ptr<json::Node>> values;

  return_on_token(json_lexer::ARRAY_END);

  while (true) {
    values.push_back(std::move(parse_all(start, end)));

    return_on_token(json_lexer::ARRAY_END);

    if ((*start++).type != json_lexer::COMMA)
      throw std::runtime_error("Expected comma or end of array.");
  }
}

std::unordered_map<std::string, std::unique_ptr<json::Node>>
parse_object(iter &start, iter &end) {
  std::unordered_map<std::string, std::unique_ptr<json::Node>> values;

  return_on_token(json_lexer::OBJ_END);

  while (true) {
    // read key
    json_lexer::Token key = (*start++);
    if (key.type != json_lexer::STRING)
      throw std::runtime_error("Invalid object key type.");

    // read colon
    check_end();
    if ((*start++).type != json_lexer::COLON)
      throw std::runtime_error("Invalid object key value separator.");

    // read value
    values.insert({key.value, std::move(parse_all(start, end))});

    return_on_token(json_lexer::OBJ_END);

    if ((*start).type != json_lexer::COMMA)
      throw std::runtime_error("Expected comma or end of object.");
  }
}

json::ConstantType parse_constant(iter &start) {
  std::string type_str = (*start).value;
  start++;

  if (type_str == "null")
    return json::JNULL;
  if (type_str == "true")
    return json::TRUE;
  if (type_str == "false")
    return json::FALSE;

  throw std::runtime_error("Invalid constant type.");
}

std::unique_ptr<json::Node> parse_all(iter &start, iter &end) {
  check_end();

  switch ((*start).type) {
  case json_lexer::ARRAY_START:
    return std::make_unique<json::Array>(parse_array(++start, end));

  case json_lexer::OBJ_START:
    return std::make_unique<json::Object>(parse_object(++start, end));

  case json_lexer::STRING:
    return std::make_unique<json::String>((*start++).value);

  case json_lexer::NUMBER:
    return std::make_unique<json::Number>(std::stod((*start++).value));

  case json_lexer::CONSTANT:
    return std::make_unique<json::Constant>(parse_constant(start));

  default:
    throw std::runtime_error("Unexpected token while parsing JSON.");
  }
}

std::unique_ptr<json::Node> parse(std::vector<json_lexer::Token> tokens) {
  iter start = tokens.begin();
  iter end = tokens.end();

  auto output = parse_all(start, end);

  if (start != end)
    throw std::runtime_error("Could not parse all JSON tokens.");

  return output;
}

} // namespace json_parser
