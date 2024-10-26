#include "query-eval.h"
#include "json.h"
#include <string>

const JsonNode &QueryEval::eval(const JsonNode &rootNode, Iter &start, Iter &end) {
  if (start == end)
    return rootNode;

  return eval_identifier(rootNode, rootNode, start, end);
}

const JsonNode &QueryEval::eval_identifier(const JsonNode &rootNode, const JsonNode &node, Iter &start, Iter &end) {
  if ((*start).type != QueryLexer::IDENTIFIER)
    throw new std::runtime_error("Error: expected an identifier");

  auto newNode = &node[(*start).value];

  return eval_access_specifier(rootNode, *newNode, ++start, end);
}

const JsonNode &QueryEval::eval_access_specifier(const JsonNode &rootNode, const JsonNode &node, Iter &start, Iter &end) {
  if (start == end)
    return node;

  if ((*start).type == QueryLexer::DOT)
    return eval_identifier(rootNode, node, ++start, end);
}