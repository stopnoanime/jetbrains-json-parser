#include "json.h"
#include <sstream>
#include <limits>

JsonArray::JsonArray(std::vector<std::shared_ptr<JsonNode>> init) : JsonNode(JsonNode::JSON_ARRAY) {
  values = init;
}

void JsonArray::serialize(std::ostream& os) const {
  os<<"[ ";
  
  for(auto it = values.begin(); it != values.end(); ++it) {
    os<<**it;
    if (it != values.end() - 1)
      os<<", ";
  }
  
  os<<" ]";
}

const JsonNode &JsonArray::operator[](int index) const {
  return *values[index];
}

double JsonArray::getMin() const {
  double min = std::numeric_limits<double>::max();
  
  for(auto it = values.begin(); it != values.end(); ++it) {
    if ((*it)->getType() != JsonNode::JSON_NUMBER)
      throw std::runtime_error("Error: array must contain numbers");
    
    min = std::min(min, static_cast<const JsonNumber &>(**it).getValue());
  }
  
  return min;
}

double JsonArray::getMax() const {
  double max = std::numeric_limits<double>::min();
  
  for(auto it = values.begin(); it != values.end(); ++it) {
    if ((*it)->getType() != JsonNode::JSON_NUMBER)
      throw std::runtime_error("Error: array must contain numbers");
    
    max = std::max(max, static_cast<const JsonNumber &>(**it).getValue());
  }
  
  return max;
}

size_t JsonArray::getSize() const {
  return values.size();
}


JsonObject::JsonObject(std::unordered_map<std::string, std::shared_ptr<JsonNode>> init) : JsonNode(JsonNode::JSON_OBJECT) {
  values = init;
}

void JsonObject::serialize(std::ostream& os) const {
  os<<"{ ";
  
  for(auto it = values.begin(); it != values.end(); ++it) {
    os<<it->first<<": "<<*it->second;
    if (std::next(it) != values.end())
      os<<", ";
  }
  
  os<<" }";
}

const JsonNode& JsonObject::operator[](std::string index) const {
  return *values.at(index);
}

size_t JsonObject::getSize() const {
  return values.size();
}

JsonString::JsonString(std::string init) : JsonNode(JsonNode::JSON_STRING) {
  value = init;
}

void JsonString::serialize(std::ostream& os) const {
  os<<'"'<<value<<'"';
}

std::string JsonString::getValue() const {
  return value;
}


JsonNumber::JsonNumber(double init) : JsonNode(JsonNode::JSON_NUMBER) {
  value = init;
}

void JsonNumber::serialize(std::ostream& os) const {
  os<<value;
}

double JsonNumber::getValue() const {
  return value;
}


JsonConstant::JsonConstant(ConstantType init) : JsonNode(JsonNode::JSON_CONSTANT) {
  value = init;
}

void JsonConstant::serialize(std::ostream& os) const {
  os<<(value == JSON_NULL ? "null" : value == JSON_TRUE ? "true" : "false");
}