#include "json.h"
#include <limits>
#include <sstream>
#include <stdexcept>

namespace json {

// Node

Node::Node(NodeType type) : type(type) {}

NodeType Node::getType() const { return type; }

void Node::serialize(std::ostream &os) const { os << "Node"; }

std::string Node::to_string() const {
  std::ostringstream oss;
  serialize(oss);
  return oss.str();
}

// Array

Array::Array(std::vector<std::unique_ptr<Node>> init)
    : Node(ARRAY), values(std::move(init)) {}

void Array::serialize(std::ostream &os) const {
  os << "[ ";

  for (auto it = values.begin(); it != values.end(); ++it) {
    os << **it;
    if (it != values.end() - 1)
      os << ", ";
  }

  os << " ]";
}

const Node &Array::operator[](int index) const {
  if (index < 0 || (unsigned)index >= values.size())
    throw std::runtime_error("Index: " + std::to_string(index) +
                             " is out of bounds for array.");

  return *values[index];
}

double Array::getMinMax(bool isMin) const {
  double res = isMin ? std::numeric_limits<double>::max()
                     : std::numeric_limits<double>::min();

  for (auto &value : values) {
    if (value->getType() != NUMBER)
      throw std::runtime_error(
          "Array must only contain numbers to get min/max.");

    double val = static_cast<const Number &>(*value).getValue();
    res = isMin ? std::min(res, val) : std::max(res, val);
  }

  return res;
}

size_t Array::getSize() const { return values.size(); }

// Object

Object::Object(std::unordered_map<std::string, std::unique_ptr<Node>> init)
    : Node(OBJECT), values(std::move(init)) {}

void Object::serialize(std::ostream &os) const {
  os << "{ ";

  for (auto it = values.begin(); it != values.end(); ++it) {
    os << it->first << ": " << *it->second;
    if (std::next(it) != values.end())
      os << ", ";
  }

  os << " }";
}

const Node &Object::operator[](std::string index) const {
  auto it = values.find(index);
  if (it == values.end())
    throw std::runtime_error("Key: " + index + " not found in object.");

  return *it->second;
}

size_t Object::getSize() const { return values.size(); }

// String

String::String(std::string init) : Node(STRING), value(init) {}

void String::serialize(std::ostream &os) const { os << '"' << value << '"'; }

std::string String::getValue() const { return value; }

// Number

Number::Number(double init) : Node(NUMBER), value(init) {}

void Number::serialize(std::ostream &os) const { os << value; }

double Number::getValue() const { return value; }

// Constant

Constant::Constant(ConstantType init) : Node(CONSTANT), value(init) {}

void Constant::serialize(std::ostream &os) const {
  os << (value == JNULL ? "null" : value == TRUE ? "true" : "false");
}

} // namespace json