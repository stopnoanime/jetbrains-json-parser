#include "query-eval.h"

namespace query_eval {

const json::Node &eval_identifier(const json::Node &rootNode,
                                  const json::Node &node, iter &start,
                                  iter &end) {
  if (start == end || (*start).type != query_lexer::IDENTIFIER)
    throw new std::runtime_error("Error: expected an identifier");

  if (node.getType() != json::OBJECT)
    throw new std::runtime_error(
        "Error: node type does not support identifier");

  auto &newNode = static_cast<const json::Object &>(node)[(*start).value];
  return eval_access_specifier(rootNode, newNode, ++start, end);
}

const json::Node &eval_access_specifier(const json::Node &rootNode,
                                        const json::Node &node, iter &start,
                                        iter &end) {
  if (start == end)
    return node;

  if ((*start).type == query_lexer::DOT)
    return eval_identifier(rootNode, node, ++start, end);

  if ((*start).type == query_lexer::SUBSCRIPT_START)
    return eval_subscript(rootNode, node, ++start, end);

  return node;
}

const json::Node &eval_subscript(const json::Node &rootNode,
                                 const json::Node &node, iter &start,
                                 iter &end) {
  if (start == end)
    throw new std::runtime_error("Error: expected a subscript");

  auto &subscriptNode = eval_all(rootNode, start, end);

  if ((*start++).type != query_lexer::SUBSCRIPT_END)
    throw new std::runtime_error("Error: expected a subscript end");

  // Check if subscriptNode's value matches to node's type
  if (subscriptNode.getType() == json::NUMBER) {
    if (node.getType() != json::ARRAY)
      throw new std::runtime_error("Error: subscript must be a number");

    return eval_access_specifier(
        rootNode,
        static_cast<const json::Array &>(
            node)[static_cast<const json::Number &>(subscriptNode).getValue()],
        start, end);
  }

  throw new std::runtime_error(
      "Error: node type does not support subscripting");
}

const json::Node &eval_function(const json::Node &rootNode, iter &start,
                                iter &end) {
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

const json::Node &eval_minmax(const json::Node &rootNode, iter &start,
                              iter &end, bool isMin) {
  if (start == end)
    throw new std::runtime_error("Error: expected an argument");

  double minmaxValue = isMin ? std::numeric_limits<double>::max()
                             : std::numeric_limits<double>::min();

  while (true) {
    auto &argNode = eval_all(rootNode, start, end);
    double value;

    if (argNode.getType() == json::ARRAY)
      value = static_cast<const json::Array &>(argNode).getMinMax(isMin);
    else if (argNode.getType() == json::NUMBER)
      value = static_cast<const json::Number &>(argNode).getValue();
    else
      throw new std::runtime_error(
          "Error: argument must be a number, or array of numbers");

    minmaxValue =
        isMin ? std::min(minmaxValue, value) : std::max(minmaxValue, value);

    if ((*start).type == query_lexer::ARGS_END) {
      start++;
      return *(new json::Number(minmaxValue));
    } else if ((*start++).type != query_lexer::COMMA) {
      throw new std::runtime_error(
          "Error: expected a comma or an argument end");
    }
  }
}

const json::Node &eval_size(const json::Node &rootNode, iter &start,
                            iter &end) {
  if (start == end)
    throw new std::runtime_error("Error: expected an argument");

  auto &argNode = eval_all(rootNode, start, end);
  size_t size;

  if (argNode.getType() == json::ARRAY)
    size = static_cast<const json::Array &>(argNode).getSize();
  else if (argNode.getType() == json::OBJECT)
    size = static_cast<const json::Object &>(argNode).getSize();
  else if (argNode.getType() == json::STRING)
    size = static_cast<const json::String &>(argNode).getValue().size();
  else
    throw new std::runtime_error(
        "Error: argument must be an array or object or string");

  if ((*start).type == query_lexer::ARGS_END) {
    start++;
    return *(new json::Number(size));
  }

  throw new std::runtime_error("Error: expected an argument end");
}

const json::Node &eval_all(const json::Node &rootNode, iter &start, iter &end) {
  if (start == end)
    return rootNode;

  if ((*start).type == query_lexer::NUMBER)
    return *(new json::Number(std::stod((*start++).value)));

  if ((*start).type != query_lexer::IDENTIFIER)
    throw new std::runtime_error(
        "Error: Query must start with an identifier or function call");

  if ((start + 1) != end && (*(start + 1)).type == query_lexer::ARGS_START)
    return eval_function(rootNode, start, end);

  return eval_identifier(rootNode, rootNode, start, end);
}

const json::Node &eval(const json::Node &rootNode,
                       std::vector<query_lexer::Token> tokens) {
  iter start = tokens.begin();
  iter end = tokens.end();

  auto &output = eval_all(rootNode, start, end);

  if (start != end)
    throw std::runtime_error("Unexpected token.");

  return output;
}

} // namespace query_eval