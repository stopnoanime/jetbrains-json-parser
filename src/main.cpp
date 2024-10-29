#include "input/json-lexer.h"
#include "input/json-parser.h"
#include "query/query-eval.h"
#include "query/query-lexer.h"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Please provide two argumnets: <file.json> \"query\"\n";
    return -1;
  }

  std::ifstream file(argv[1]);

  if (!file) {
    std::cout << "Specified file does not exists.\n";
    return -1;
  }

  std::string query = argv[2];
  std::string json((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());

  try {
    auto rootNode = json_parser::parse(json_lexer::lex(json));
    std::cout << query_eval::eval(*rootNode, query_lexer::lex(query)) << "\n";
  } catch (const std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << '\n';
  }

  return 0;
}