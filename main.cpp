#include "lexer.h"
#include "parser.h"
#include "query-eval.h"
#include "query-lexer.h"
#include <algorithm>
#include <iostream>
#include <string>

int main() {
  std::string json = R"({"a": { "b": [ 1, 2, { "c": "testd" }, [11, 12] ]}})";
  std::string query = "a.b[max(1,3)][0]";

  try {
    auto out = lexer(json);

    auto start = out.begin();
    auto end = out.end();
    auto rootNode = parse(start, end);

    std::cout << *rootNode << std::endl;

    auto query_out = QueryLexer().lex(query);
    auto query_start = query_out.begin();
    auto query_end = query_out.end();

    const JsonNode &query_eval =
        QueryEval().eval(*rootNode, query_start, query_end);

    std::cout << query_eval << std::endl;
  } catch (const std::runtime_error &e) {
    std::cerr <<"aaa"<< e.what() << '\n';
  }
}