#include "query-eval.h"
#include "json.h"
#include <limits>
#include <memory>
#include <string>

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
    return eval_function(rootNode, start, end);

  return eval_identifier(rootNode, rootNode, start, end);
}

const JsonNode &QueryEval::eval_identifier(const JsonNode &rootNode,
                                           const JsonNode &node, Iter &start,
                                           Iter &end) {
  if (start == end || (*start).type != QueryLexer::IDENTIFIER)
    throw new std::runtime_error("Error: expected an identifier");

  if (node.getType() != JsonNode::JSON_OBJECT)
    throw new std::runtime_error(
        "Error: node type does not support identifier");

  auto &newNode = static_cast<const JsonObject &>(node)[(*start).value];
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

  return node;
}

const JsonNode &QueryEval::eval_subscript(const JsonNode &rootNode,
                                          const JsonNode &node, Iter &start,
                                          Iter &end) {
  if (start == end)
    throw new std::runtime_error("Error: expected a subscript");

  auto &subscriptNode = eval(rootNode, start, end);

  if ((*start++).type != QueryLexer::SUBSCRIPT_END)
    throw new std::runtime_error("Error: expected a subscript end");

  // Check if subscriptNode's value matches to node's type
  if (subscriptNode.getType() == JsonNode::JSON_NUMBER) {
    if (node.getType() != JsonNode::JSON_ARRAY)
      throw new std::runtime_error("Error: subscript must be a number");

    return eval_access_specifier(
        rootNode,
        static_cast<const JsonArray &>(
            node)[static_cast<const JsonNumber &>(subscriptNode).getValue()],
        start, end);
  }

  throw new std::runtime_error(
      "Error: node type does not support subscripting");
}

const JsonNode &QueryEval::eval_function(const JsonNode &rootNode, Iter &start,
                                         Iter &end) {
  std::string functionName = (*start++).value;
  start++; // Skip '('

  if (functionName == "min" || functionName == "max") {
    return eval_minmax(rootNode, start, end, functionName == "min");
  } else if (functionName == "size") {
    return eval_size(rootNode, start, end);
  } else {
    throw new std::runtime_error("Error: unknown function name");
  }
}

const JsonNode &QueryEval::eval_minmax(const JsonNode &rootNode, Iter &start,
                                       Iter &end, bool isMin) {
  if (start == end)
    throw new std::runtime_error("Error: expected an argument");

  double minmaxValue = isMin ? std::numeric_limits<double>::max()
                             : std::numeric_limits<double>::min();

  while (true) {
    auto &argNode = eval(rootNode, start, end);

    if (argNode.getType() == JsonNode::JSON_ARRAY) {
      if (isMin) {
        minmaxValue = std::min(
            minmaxValue, static_cast<const JsonArray &>(argNode).getMin());
      } else {
        minmaxValue = std::max(
            minmaxValue, static_cast<const JsonArray &>(argNode).getMax());
      }
    } else if (argNode.getType() == JsonNode::JSON_NUMBER) {
      if (isMin) {
        minmaxValue = std::min(
            minmaxValue, static_cast<const JsonNumber &>(argNode).getValue());
      } else {
        minmaxValue = std::max(
            minmaxValue, static_cast<const JsonNumber &>(argNode).getValue());
      }
    } else {
      throw new std::runtime_error(
          "Error: argument must be a number, or array of numbers");
    }

    if ((*start).type == QueryLexer::ARGS_END) {
      start++;
      return *(new JsonNumber(minmaxValue));
    } else if ((*start++).type != QueryLexer::COMMA) {
      throw new std::runtime_error(
          "Error: expected a comma or an argument end");
    }
  }
}

const JsonNode &QueryEval::eval_size(const JsonNode &rootNode, Iter &start,
                                     Iter &end) {
  if (start == end)
    throw new std::runtime_error("Error: expected an argument");

  auto &argNode = eval(rootNode, start, end);
  size_t size;

  if (argNode.getType() == JsonNode::JSON_ARRAY)
    size = static_cast<const JsonArray &>(argNode).getSize();
  else if (argNode.getType() == JsonNode::JSON_OBJECT)
    size = static_cast<const JsonObject &>(argNode).getSize();
  else if (argNode.getType() == JsonNode::JSON_STRING)
    size = static_cast<const JsonString &>(argNode).getValue().size();
  else
    throw new std::runtime_error(
        "Error: argument must be an array or object or string");

  if ((*start).type == QueryLexer::ARGS_END) {
    start++;
    return *(new JsonNumber(size));
  }

  throw new std::runtime_error("Error: expected an argument end");
}