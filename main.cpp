#include <string>
#include <algorithm>
#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "query-eval.h"
#include "query-lexer.h"

int main() {
  std::string json = R"({"d": 1, "a": { "b": [ 1, 2, { "c": "test", "ef" : 1 }, [11, 12] ]}})";

  auto out = lexer(json);

  auto start = out.begin();
  auto end = out.end();
  auto rootNode = parse(start,end);
  
  std::cout<<*rootNode<<std::endl;

  std::string query = "a.b";
  auto query_out = QueryLexer().lex(query);
  auto query_start = query_out.begin();
  auto query_end = query_out.end();

  auto query_eval = &QueryEval().eval(*rootNode, query_start, query_end);

  std::cout<<*query_eval<<std::endl;
}