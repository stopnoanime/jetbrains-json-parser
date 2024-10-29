#pragma once

#include <string>

#include "../json/json.h"
#include "query-lexer.h"

namespace query_eval {

typedef std::vector<query_lexer::Token>::iterator iter;

enum class ResultType { NODE, NUMBER };

struct Result {
  ResultType type;
  const json::Node *node;
  double number;
};

Result eval_all(const json::Node &rootNode, iter &start, iter &end);

std::string eval(const json::Node &rootNode,
                 std::vector<query_lexer::Token> tokens);

}; // namespace query_eval
