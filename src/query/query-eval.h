#ifndef QUERY_EVAL_H
#define QUERY_EVAL_H

#include <limits>
#include <memory>
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

const json::Node &eval_identifier(const json::Node &rootNode,
                                  const json::Node &node, iter &start,
                                  iter &end);

const json::Node &eval_access_specifier(const json::Node &rootNode,
                                        const json::Node &node, iter &start,
                                        iter &end);

Result eval_all(const json::Node &rootNode, iter &start, iter &end);

std::string eval(const json::Node &rootNode,
                    std::vector<query_lexer::Token> tokens);

}; // namespace query_eval

#endif
