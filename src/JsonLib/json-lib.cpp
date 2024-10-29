#include "json-lib.h"
#include "input/json-lexer.h"
#include "input/json-parser.h"
#include "query/query-eval.h"
#include "query/query-lexer.h"
#include <stdexcept>

std::string eval_json_query(const std::string &json, const std::string &query) {
  try {
    auto rootNode = json_parser::parse(json_lexer::lex(json));
    return query_eval::eval(*rootNode, query_lexer::lex(query));
  } catch (const std::runtime_error &e) {
    return "Error: " + std::string(e.what());
  }
}