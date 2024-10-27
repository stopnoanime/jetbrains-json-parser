#ifndef QUERY_EVAL_H
#define QUERY_EVAL_H

#include "json.h"
#include "query-lexer.h"

class QueryEval {
  typedef std::vector<QueryLexer::Token>::iterator Iter;

  static const JsonNode &eval_subscript(const JsonNode &rootNode,
                                        const JsonNode &node, Iter &start,
                                        Iter &end);
  static const JsonNode &eval_function(const JsonNode &rootNode, Iter &start,
                                       Iter &end);
  
  static const JsonNode &eval_minmax(const JsonNode &rootNode, Iter &start,
                                       Iter &end, bool isMin);
  static const JsonNode &eval_size(const JsonNode &rootNode, Iter &start,
                                       Iter &end);
  static const JsonNode &eval_identifier(const JsonNode &rootNode,
                                         const JsonNode &node, Iter &start,
                                         Iter &end);
  static const JsonNode &eval_access_specifier(const JsonNode &rootNode,
                                               const JsonNode &node,
                                               Iter &start, Iter &end);

public:
  static const JsonNode &eval(const JsonNode &rootNode,Iter &start, Iter &end);
};

#endif
