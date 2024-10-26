#include <string>
#include <algorithm>
#include <iostream>
#include "lexer.h"
#include "parser.h"
#include <unordered_map>

int main() {
  std::string json = R"({"a": { "b": [ 1, 2, { "c": "test", "ef" : 1 }, [11, 12] ]}})";

  auto out = lexer(json);

  //for (auto i : out)
  //  std::cout<<TokenTypeToString.at(i.type)<<" "<<i.value<<"\n";

  auto start = out.begin();
  auto end = out.end();

  auto a = JsonNode().create(start, end);

  while (true)
  {
    /* code */
  }
  
}