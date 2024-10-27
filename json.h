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

  JsonNode(NodeType init) : type(init) {}
  NodeType getType() const { return type; };
  virtual void serialize(std::ostream &os) const {};

  friend std::ostream &operator<<(std::ostream &os, const JsonNode &node) {
    node.serialize(os);
    return os;
  }

private:
  NodeType type;
};

class JsonArray : public JsonNode {
private:
  std::vector<std::shared_ptr<JsonNode>> values;

public:
  JsonArray(std::vector<std::shared_ptr<JsonNode>> init);
  void serialize(std::ostream &os) const;

  const JsonNode &operator[](int index) const;
  double getMin() const;
  double getMax() const;
  size_t getSize() const;
};

class JsonObject : public JsonNode {
private:
  std::unordered_map<std::string, std::shared_ptr<JsonNode>> values;

public:
  JsonObject(std::unordered_map<std::string, std::shared_ptr<JsonNode>> init);
  void serialize(std::ostream &os) const;

  const JsonNode &operator[](std::string index) const;
  size_t getSize() const;
};

class JsonString : public JsonNode {
private:
  std::string value;

public:
  JsonString(std::string init);
  void serialize(std::ostream &os) const;

  std::string getValue() const;
};

class JsonNumber : public JsonNode {
private:
  double value;

public:
  JsonNumber(double init);
  void serialize(std::ostream &os) const;

  double getValue() const;
};

class JsonConstant : public JsonNode {
public:
  enum ConstantType { JSON_NULL, JSON_TRUE, JSON_FALSE };
  
  JsonConstant(ConstantType init);
  void serialize(std::ostream &os) const;

private:
  ConstantType value;
};

#endif
