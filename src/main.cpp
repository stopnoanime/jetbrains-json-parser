#include "input/json-lexer.h"
#include "input/json-parser.h"
#include "query/query-lexer.h"
#include "query/query-eval.h"

#include <iostream>
#include <string>

int main() {
  std::string json = R"({"a": { "b": [ 1, 2, { "c": "testd" }, [11, 12] ]}})";
  std::string query = "a.b[max(1,3)][0]";

  try {
    auto rootNode = json_parser::parse(json_lexer::lex(json));
    std::cout << *rootNode << std::endl;

    auto queryOut = &query_eval::eval(*rootNode, query_lexer::lex(query));
    std::cout << *queryOut << std::endl;
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << '\n';
  }
}