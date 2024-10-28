#include "json.h"
#include <limits>
#include <sstream>

namespace json {

// Node

Node::Node(NodeType type) : type(type) {}

NodeType Node::getType() const { return type; }

void Node::serialize(std::ostream &os) const { os << "Node"; }

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

const Node &Array::operator[](int index) const { return *values[index]; }

double Array::getMinMax(bool isMin) const {
  double minmaxValue = isMin ? std::numeric_limits<double>::max()
                             : std::numeric_limits<double>::min();

  for (auto it = values.begin(); it != values.end(); ++it) {
    if ((*it)->getType() != NUMBER)
      throw std::runtime_error("Error: array must contain numbers");

    double val = static_cast<const Number &>(**it).getValue();
    minmaxValue =
        isMin ? std::min(minmaxValue, val) : std::max(minmaxValue, val);
  }

  return minmaxValue;
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
  return *values.at(index);
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