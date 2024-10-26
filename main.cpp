#include <string>
#include <algorithm>
#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "eval.h"

int main() {
  std::string json = R"({"d": 1, "a": { "b": [ 1, 2, { "c": "test", "ef" : 1 }, [11, 12] ]}})";

  auto out = lexer(json);

  auto start = out.begin();
  auto end = out.end();
  auto a = parse(start,end);
  
  std::cout<<*a<<std::endl;

  std::cout<<eval(*a, "a.b")<<std::endl;
}