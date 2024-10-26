#ifndef JSON_H
#define JSON_H

#include "lexer.h"
#include <memory>
#include <unordered_map>
#include <vector>

class JsonNode {
public:
  enum NodeType {
    JSON_ARRAY,
    JSON_OBJECT,
    JSON_STRING,
    JSON_NUMBER,
    JSON_CONSTANT
  };

  virtual NodeType getType() const = 0;
  virtual void serialize(std::ostream &os) const = 0;
  virtual const JsonNode &operator[](std::string index) const {
    throw new std::runtime_error("Error: trying to access a field: " + index +
                                 " of a non-object");
  }

  friend std::ostream &operator<<(std::ostream &os, const JsonNode &node) {
    node.serialize(os);
    return os;
  }
};

class JsonArray : public JsonNode {
private:
  std::vector<std::shared_ptr<JsonNode>> values;

public:
  JsonArray(std::vector<std::shared_ptr<JsonNode>> init);
  void serialize(std::ostream &os) const;
  NodeType getType() const;
};

class JsonObject : public JsonNode {
private:
  std::unordered_map<std::string, std::shared_ptr<JsonNode>> values;

public:
  JsonObject(std::unordered_map<std::string, std::shared_ptr<JsonNode>> init);
  void serialize(std::ostream &os) const;
  NodeType getType() const;

  const JsonNode &operator[](std::string index) const;
};

class JsonString : public JsonNode {
private:
  std::string value;

public:
  JsonString(std::string init);
  void serialize(std::ostream &os) const;
  NodeType getType() const;
};

class JsonNumber : public JsonNode {
private:
  double value;

public:
  JsonNumber(double init);
  void serialize(std::ostream &os) const;
  NodeType getType() const;
};

class JsonConstant : public JsonNode {
public:
  enum ConstantType { JSON_NULL, JSON_TRUE, JSON_FALSE };
  JsonConstant(ConstantType init);
  void serialize(std::ostream &os) const;
  NodeType getType() const;

private:
  ConstantType value;
};

#endif
