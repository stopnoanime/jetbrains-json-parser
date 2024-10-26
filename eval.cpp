#include <string>
#include "json.h"

const JsonNode& eval(const JsonNode &node, std::string str) {
  if(str.empty())
    return node;

  if(node.getType() != JsonNode::JSON_OBJECT)
    throw std::runtime_error("Error: trying to access a field of a non-object");

  std::string key = str.substr(0, str.find('.'));

  std::string rest = str.substr(str.find('.') + 1);
  if(str.find('.') == std::string::npos) rest = "";

  return eval(node[key], rest);
}
