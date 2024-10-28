#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <vector>
#include <memory>

#include "../json/json.h"
#include "json-lexer.h"

namespace json_parser {

typedef std::vector<json_lexer::Token>::iterator iter;

std::unique_ptr<json::Node> parse_all(iter &start, iter &end);
std::unique_ptr<json::Node> parse(std::vector<json_lexer::Token> tokens);

} // namespace json_parser

#endif
