#include "parser.h"
#include "lexer.h"
#include <memory>

std::vector<std::shared_ptr<JsonNode>> parse_array(Iter &start, Iter &end) {
  std::vector<std::shared_ptr<JsonNode>> values;

  if ((*start++).type != ARRAY_START)
    throw std::runtime_error("Invalid array start.");

  if ((*start).type == ARRAY_END) {
    start++;
    return values;
  }

  while (true) {
    values.push_back(parse(start, end));

    if ((*start).type == COMMA)
      start++;
    else if ((*start).type == ARRAY_END) {
      start++;
      return values;
    } else
      throw std::runtime_error("Expected comma or end of array.");
  }
}

std::unordered_map<std::string, std::shared_ptr<JsonNode>>
parse_object(Iter &start, Iter &end) {
  std::unordered_map<std::string, std::shared_ptr<JsonNode>> values;

  if ((*start++).type != OBJ_START)
    throw std::runtime_error("Invalid object start.");

  if ((*start).type == OBJ_END) {
    start++;
    return values;
  }

  while (true) {
    Token key = (*start++);

    if (key.type != STRING)
      throw std::runtime_error("Invalid object key");

    if ((*start++).type != COLON)
      throw std::runtime_error("Invalid object key value seperator");

    values.insert({key.value, parse(start, end)});

    if ((*start).type == OBJ_END) {
      start++;
      return values;
    } else if ((*start).type == COMMA)
      start++;
    else
      throw std::runtime_error("Expected comma or end of object.");
  }
}

JsonConstant::ConstantType parse_constant(Iter &start) {
  std::string type_str = (*start).value;
  start++;

  return type_str == "null"   ? JsonConstant::ConstantType::JSON_NULL
         : type_str == "true" ? JsonConstant::ConstantType::JSON_TRUE
                              : JsonConstant::ConstantType::JSON_FALSE;
}

std::shared_ptr<JsonNode> parse(Iter &start, Iter &end) {
  switch ((*start).type) {
  case ARRAY_START:
    return std::make_shared<JsonArray>(parse_array(start, end));

  case OBJ_START:
    return std::make_shared<JsonObject>(parse_object(start, end));

  case STRING:
    return std::make_shared<JsonString>((*start++).value);

  case NUMBER:
    return std::make_shared<JsonNumber>(std::stod((*start++).value));

  case CONSTANT:
    return std::make_shared<JsonConstant>(parse_constant(start));

  default:
    throw std::runtime_error("Unexpected token.");
  }
}
