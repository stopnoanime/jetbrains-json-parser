#include "query-eval.h"
#include "json.h"
#include <string>
#include <memory>

const JsonNode &QueryEval::eval(const JsonNode &rootNode, Iter &start,
                                Iter &end) {
  if (start == end)
    return rootNode;

  if ((*start).type == QueryLexer::NUMBER)
    return *(new JsonNumber(std::stod((*start++).value)));

  if ((*start).type != QueryLexer::IDENTIFIER)
    throw new std::runtime_error(
        "Error: Query must start with an identifier or function call");

  if ((start + 1) != end && (*(start + 1)).type == QueryLexer::ARGS_START)
    return eval_function(rootNode, rootNode, start, end);

  return eval_identifier(rootNode, rootNode, start, end);
}

const JsonNode &QueryEval::eval_identifier(const JsonNode &rootNode,
                                           const JsonNode &node, Iter &start,
                                           Iter &end) {
  if (start == end || (*start).type != QueryLexer::IDENTIFIER)
    throw new std::runtime_error("Error: expected an identifier");

  if ( node.getType() != JsonNode::JSON_OBJECT)
    throw new std::runtime_error("Error: node type does not support identifier");

  auto &newNode = static_cast<const JsonObject&>(node)[(*start).value];
  return eval_access_specifier(rootNode, newNode, ++start, end);
}

const JsonNode &QueryEval::eval_access_specifier(const JsonNode &rootNode,
                                                 const JsonNode &node,
                                                 Iter &start, Iter &end) {
  if (start == end)
    return node;

  if ((*start).type == QueryLexer::DOT)
    return eval_identifier(rootNode, node, ++start, end);

  if ((*start).type == QueryLexer::SUBSCRIPT_START)
    return eval_subscript(rootNode, node, ++start, end);

  throw new std::runtime_error("Error: expected an access_specifier");
}

const JsonNode &QueryEval::eval_subscript(const JsonNode &rootNode,
                                          const JsonNode &node, Iter &start,
                                          Iter &end) {
  if (start == end)
    throw new std::runtime_error("Error: expected a subscript");

  auto &subscriptNode = eval(rootNode, start, end);

  if ((*start).type != QueryLexer::SUBSCRIPT_END)
    throw new std::runtime_error("Error: expected a subscript end");

  // Check if subscriptNode's value matches to node's type
  if (subscriptNode.getType() == JsonNode::JSON_NUMBER) {
    if(node.getType() != JsonNode::JSON_ARRAY)
      throw new std::runtime_error("Error: subscript must be a number");

    return static_cast<const JsonArray&>(node)[static_cast<const JsonNumber&>(subscriptNode).getValue()];
  }

  if (subscriptNode.getType() == JsonNode::JSON_STRING) {
    if(node.getType() != JsonNode::JSON_OBJECT)
      throw new std::runtime_error("Error: subscript must be a string");

    return static_cast<const JsonObject&>(node)[static_cast<const JsonString&>(subscriptNode).getValue()];
  }

  throw new std::runtime_error("Error: node type does not support subscripting");
}

const JsonNode &QueryEval::eval_function(const JsonNode &rootNode,
                                         const JsonNode &node, Iter &start,
                                         Iter &end) {}
