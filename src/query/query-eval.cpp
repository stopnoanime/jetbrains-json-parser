#include "query-eval.h"

namespace query_eval {

double eval_minmax(const json::Node &rootNode, iter &start, iter &end,
                   bool isMin) {
  double res = isMin ? std::numeric_limits<double>::max()
                     : std::numeric_limits<double>::min();

  while (true) {
    Result arg = eval_all(rootNode, start, end);
    double val;

    if (arg.type == ResultType::NUMBER)
      val = arg.number;
    else if (arg.type == ResultType::NODE && arg.node->getType() == json::ARRAY)
      val = static_cast<const json::Array *>(arg.node)->getMinMax(isMin);
    else if (arg.type == ResultType::NODE &&
             arg.node->getType() == json::NUMBER)
      val = static_cast<const json::Number *>(arg.node)->getValue();
    else
      throw std::runtime_error(
          "Arguments of min/max must be numbers, or arrays of numbers.");

    res = isMin ? std::min(res, val) : std::max(res, val);

    if ((*start).type == query_lexer::ARGS_END) {
      start++;
      return res;
    } else if ((*start++).type != query_lexer::COMMA) {
      throw std::runtime_error(
          "Expected a comma or function argument list end.");
    }
  }
}

double eval_size(const json::Node &rootNode, iter &start, iter &end) {
  Result arg = eval_all(rootNode, start, end);
  size_t size;

  if (arg.type == ResultType::NODE && arg.node->getType() == json::ARRAY)
    size = static_cast<const json::Array *>(arg.node)->getSize();
  else if (arg.type == ResultType::NODE && arg.node->getType() == json::OBJECT)
    size = static_cast<const json::Object *>(arg.node)->getSize();
  else if (arg.type == ResultType::NODE && arg.node->getType() == json::STRING)
    size = static_cast<const json::String *>(arg.node)->getValue().size();
  else
    throw std::runtime_error(
        "Argument of size function must be an array, object or string.");

  if ((*start).type == query_lexer::ARGS_END) {
    start++;
    return size;
  }

  throw std::runtime_error("Expected function argument list end.");
}

double eval_function(const json::Node &rootNode, iter &start, iter &end) {
  std::string functionName = (*start++).value;
  start++; // Skip '('

  if (functionName == "min" || functionName == "max")
    return eval_minmax(rootNode, start, end, functionName == "min");
  else if (functionName == "size")
    return eval_size(rootNode, start, end);

  throw std::runtime_error("Unknown query function name: " + functionName);
}

double eval_subscript(const json::Node &rootNode, const json::Node &node,
                      iter &start, iter &end) {
  if (node.getType() != json::ARRAY)
    throw std::runtime_error("Subscript is only supported on arrays.");

  Result arg = eval_all(rootNode, start, end);

  if ((*start++).type != query_lexer::SUBSCRIPT_END)
    throw std::runtime_error("Expected a query subscript end.");

  if (arg.type == ResultType::NUMBER)
    return arg.number;
  else if (arg.type == ResultType::NODE && arg.node->getType() == json::NUMBER)
    return static_cast<const json::Number *>(arg.node)->getValue();

  throw std::runtime_error("Subscript's value must be a number.");
}

const json::Node &eval_identifier(const json::Node &rootNode,
                                  const json::Node &node, iter &start,
                                  iter &end) {
  if (start == end || (*start).type != query_lexer::IDENTIFIER)
    throw std::runtime_error("Expected an object's key identifier in query.");

  if (node.getType() != json::OBJECT)
    throw std::runtime_error("Only objects support query identifier access.");

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

  if ((*start).type == query_lexer::SUBSCRIPT_START) {
    double subscript = eval_subscript(rootNode, node, ++start, end);
    auto &newNode = static_cast<const json::Array &>(node)[subscript];

    return eval_access_specifier(rootNode, newNode, start, end);
  }

  return node;
}

Result eval_all(const json::Node &rootNode, iter &start, iter &end) {
  if (start == end)
    throw std::runtime_error("Expected an query expression.");

  if ((*start).type == query_lexer::NUMBER)
    return {ResultType::NUMBER, NULL, std::stod((*start++).value)};

  if ((*start).type != query_lexer::IDENTIFIER)
    throw std::runtime_error(
        "Query must start with an identifier or function call.");

  if ((start + 1) != end && (*(start + 1)).type == query_lexer::ARGS_START)
    return {ResultType::NUMBER, NULL, eval_function(rootNode, start, end)};

  return {ResultType::NODE, &eval_identifier(rootNode, rootNode, start, end),
          0};
}

std::string eval(const json::Node &rootNode,
                 std::vector<query_lexer::Token> tokens) {
  if (tokens.empty())
    return rootNode.to_string();

  iter start = tokens.begin();
  iter end = tokens.end();

  Result output = eval_all(rootNode, start, end);

  if (start != end)
    throw std::runtime_error("Could not eval entire query.");

  if (output.type == ResultType::NUMBER)
    return std::to_string(output.number);

  return output.node->to_string();
}

} // namespace query_eval