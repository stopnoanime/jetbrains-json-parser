add_library(JsonLib STATIC
    json-lib.cpp
    input/json-lexer.cpp
    input/json-parser.cpp
    json/json.cpp
    query/query-eval.cpp
    query/query-lexer.cpp
)

target_include_directories(JsonLib PUBLIC .)
