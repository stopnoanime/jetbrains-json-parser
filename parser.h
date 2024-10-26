#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "json.h"
#include <vector>

typedef std::vector<Token>::iterator Iter;

std::shared_ptr<JsonNode> parse(Iter &start, Iter &end);

#endif
