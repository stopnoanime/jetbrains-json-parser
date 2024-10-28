#ifndef JSON_H
#define JSON_H

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

namespace json {

enum NodeType { ARRAY, OBJECT, STRING, NUMBER, CONSTANT };
enum ConstantType { JNULL, TRUE, FALSE };

class Node {
private:
  NodeType type;

public:
  Node(NodeType type);
  NodeType getType() const;

  virtual void serialize(std::ostream &os) const;
  std::string to_string() const;

  friend std::ostream &operator<<(std::ostream &os, const Node &node) {
    node.serialize(os);
    return os;
  }
};

class Array : public Node {
private:
  std::vector<std::unique_ptr<Node>> values;

public:
  Array(std::vector<std::unique_ptr<Node>> init);
  void serialize(std::ostream &os) const;

  const Node &operator[](int index) const;
  double getMinMax(bool isMin) const;
  size_t getSize() const;
};

class Object : public Node {
private:
  std::unordered_map<std::string, std::unique_ptr<Node>> values;

public:
  Object(std::unordered_map<std::string, std::unique_ptr<Node>> init);
  void serialize(std::ostream &os) const;

  const Node &operator[](std::string index) const;
  size_t getSize() const;
};

class String : public Node {
private:
  std::string value;

public:
  String(std::string init);
  void serialize(std::ostream &os) const;

  std::string getValue() const;
};

class Number : public Node {
private:
  double value;

public:
  Number(double init);
  void serialize(std::ostream &os) const;

  double getValue() const;
};

class Constant : public Node {
private:
  ConstantType value;

public:
  Constant(ConstantType init);
  void serialize(std::ostream &os) const;
};
} // namespace json

#endif
