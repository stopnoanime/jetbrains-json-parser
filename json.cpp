#include "json.h"
#include <sstream>

JsonArray::JsonArray(std::vector<std::shared_ptr<JsonNode>> init) {
  values = init;
}

JsonNode::NodeType JsonArray::getType() const {
  return JsonNode::JSON_ARRAY;
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


JsonObject::JsonObject(std::unordered_map<std::string, std::shared_ptr<JsonNode>> init) {
  values = init;
}

JsonNode::NodeType JsonObject::getType() const {
  return JsonNode::JSON_OBJECT;
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

JsonString::JsonString(std::string init) {
  value = init;
}

JsonNode::NodeType JsonString::getType() const {
  return JsonNode::JSON_STRING;
}

void JsonString::serialize(std::ostream& os) const {
  os<<'"'<<value<<'"';
}


JsonNumber::JsonNumber(double init) {
  value = init;
}

JsonNode::NodeType JsonNumber::getType() const {
  return JsonNode::JSON_NUMBER;
}

void JsonNumber::serialize(std::ostream& os) const {
  os<<value;
}


JsonConstant::JsonConstant(ConstantType init) {
  value = init;
}

JsonNode::NodeType JsonConstant::getType() const {
  return JsonNode::JSON_CONSTANT;
}

void JsonConstant::serialize(std::ostream& os) const {
  os<<(value == JSON_NULL ? "null" : value == JSON_TRUE ? "true" : "false");
}