Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests
    json-lib-tests.cpp
    input/json-lexer-tests.cpp
    input/json-parser-tests.cpp
    json/json-tests.cpp
    query/query-eval-tests.cpp
    query/query-lexer-tests.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)