#include "json-parser.h"

namespace json_parser {

std::vector<std::unique_ptr<json::Node>> parse_array(iter &start, iter &end) {
  std::vector<std::unique_ptr<json::Node>> values;

  if ((*start++).type != json_lexer::ARRAY_START)
    throw std::runtime_error("Invalid array start.");

  if ((*start).type == json_lexer::ARRAY_END) {
    start++;
    return values;
  }

  while (true) {
    values.push_back(std::move(parse_all(start, end)));

    if ((*start).type == json_lexer::COMMA)
      start++;
    else if ((*start).type == json_lexer::ARRAY_END) {
      start++;
      return values;
    } else
      throw std::runtime_error("Expected comma or end of array.");
  }
}

std::unordered_map<std::string, std::unique_ptr<json::Node>>
parse_object(iter &start, iter &end) {
  std::unordered_map<std::string, std::unique_ptr<json::Node>> values;

  if ((*start++).type != json_lexer::OBJ_START)
    throw std::runtime_error("Invalid object start.");

  if ((*start).type == json_lexer::OBJ_END) {
    start++;
    return values;
  }

  while (true) {
    json_lexer::Token key = (*start++);

    if (key.type != json_lexer::STRING)
      throw std::runtime_error("Invalid object key");

    if ((*start++).type != json_lexer::COLON)
      throw std::runtime_error("Invalid object key value seperator");

    values.insert({key.value, std::move(parse_all(start, end))});

    if ((*start).type == json_lexer::OBJ_END) {
      start++;
      return values;
    } else if ((*start).type == json_lexer::COMMA)
      start++;
    else
      throw std::runtime_error("Expected comma or end of object.");
  }
}

json::ConstantType parse_constant(iter &start) {
  std::string type_str = (*start).value;
  start++;

  return type_str == "null"   ? json::JNULL
         : type_str == "true" ? json::TRUE
                              : json::FALSE;
}

std::unique_ptr<json::Node> parse_all(iter &start, iter &end) {
  switch ((*start).type) {
  case json_lexer::ARRAY_START:
    return std::make_unique<json::Array>(parse_array(start, end));

  case json_lexer::OBJ_START:
    return std::make_unique<json::Object>(parse_object(start, end));

  case json_lexer::STRING:
    return std::make_unique<json::String>((*start++).value);

  case json_lexer::NUMBER:
    return std::make_unique<json::Number>(std::stod((*start++).value));

  case json_lexer::CONSTANT:
    return std::make_unique<json::Constant>(parse_constant(start));

  default:
    throw std::runtime_error("Unexpected token.");
  }
}

std::unique_ptr<json::Node> parse(std::vector<json_lexer::Token> tokens) {
  iter start = tokens.begin();
  iter end = tokens.end();

  auto output = parse_all(start, end);

  if(start != end)
    throw std::runtime_error("Unexpected token.");

  return output;
}

} // namespace json_parser
