#pragma once

#include <memory>
#include <vector>

#include "../json/json.h"
#include "json-lexer.h"

namespace json_parser {

typedef std::vector<json_lexer::Token>::iterator iter;

std::unique_ptr<json::Node> parse(std::vector<json_lexer::Token> tokens);

} // namespace json_parser
