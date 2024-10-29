#pragma once

#include <string>
#include "input/json-lexer.h"
#include "input/json-parser.h"
#include "json/json.h"
#include "query/query-lexer.h"
#include "query/query-eval.h"

std::string eval_json_query(const std::string &json, const std::string &query);