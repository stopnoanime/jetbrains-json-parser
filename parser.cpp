#include "parser.h"
#include "lexer.h"

JsonNode JsonNode::create(iter &start, iter &end) {
  switch ((*start).type) {
  case ARRAY_START:
    return JsonArray(start, end);
    break;

  case OBJ_START:
    return JsonObject(start, end);

  default:
    return JsonValue(start, end);
  }
}

JsonArray::JsonArray(iter &start, iter &end) {
  if ((*start++).type != ARRAY_START)
    throw std::runtime_error("Invalid array start.");

  if ((*start).type == ARRAY_END) {
    start++;
    return;
  }

  while (true) {
    values.push_back(JsonNode().create(start, end));

    if ((*start).type == COMMA)
      start++;
    else if ((*start).type == ARRAY_END) {
      start++;
      return;
    } else
      throw std::runtime_error("Expected comma or end of array.");
  }
}

JsonObject::JsonObject(iter &start, iter &end) {
  if ((*start++).type != OBJ_START)
    throw std::runtime_error("Invalid object start.");

  if ((*start).type == OBJ_END) {
    start++;
    return;
  }

  while (true) {
    Token key = (*start++);

    if (key.type != STRING)
      throw std::runtime_error("Invalid object key");

    if ((*start++).type != COLON)
      throw std::runtime_error("Invalid object key value seperator");

    values.insert({key.value, JsonNode().create(start, end)});

    if ((*start).type == OBJ_END) {
      start++;
      return;
    } else if ((*start).type == COMMA)
      start++;
    else
      throw std::runtime_error("Expected comma or end of object.");
  }
}

JsonValue::JsonValue(iter &start, iter &end) {
  if ((*start).type == STRING)
    value = (*start).value;
  else if ((*start).type == NUMBER)
    value = (*start).value;
  else
    throw std::runtime_error("Unexpected value");

  start++;
}