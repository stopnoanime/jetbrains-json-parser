#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <unordered_map>
#include <vector>

typedef std::vector<Token>::iterator iter;

class JsonNode {
public:
  static JsonNode create(iter &start, iter &end);
};

class JsonArray : public JsonNode {
private:
  std::vector<JsonNode> values;

public:
  JsonArray(iter &start, iter &end);
};

class JsonObject : public JsonNode {
private:
  std::unordered_map<std::string, JsonNode> values;

public:
  JsonObject(iter &start, iter &end);
};

class JsonValue : public JsonNode {
private:
  std::string value;

public:
  JsonValue(iter &start, iter &end);
};

#endif
