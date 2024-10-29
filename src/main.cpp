#include <fstream>
#include <iostream>
#include <string>
#include "json-lib.h"

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

  std::cout << eval_json_query(json, query) << "\n";

  return 0;
}