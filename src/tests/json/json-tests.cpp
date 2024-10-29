#include "json-lib.h"
#include <catch2/catch_all.hpp>

TEST_CASE("Test json::Array") {
  std::vector<std::unique_ptr<json::Node>> values;
  values.push_back(std::make_unique<json::Number>(1));
  values.push_back(std::make_unique<json::Number>(2));
  values.push_back(std::make_unique<json::Number>(3));
  json::Array array(std::move(values));

  SECTION("Test json::Array::getType") {
    REQUIRE(array.getType() == json::ARRAY);
  }

  SECTION("Test json::Array::to_string") {
    REQUIRE(array.to_string() == "[ 1, 2, 3 ]");
  }

  SECTION("Test json::Array::operator[]") {
    REQUIRE(array[2].getType() == json::NUMBER);
    REQUIRE_THROWS(array[3]);
  }

  SECTION("Test json::Array::getMinMax") {
    REQUIRE(array.getMinMax(true) == 1);
    REQUIRE(array.getMinMax(false) == 3);
  }

  SECTION("Test json::Array::getMinMax with non-number") {
    values.push_back(std::make_unique<json::String>("a"));
    json::Array array2(std::move(values));

    REQUIRE_THROWS(array2.getMinMax(true));
  }

  SECTION("Test json::Array::getSize") { REQUIRE(array.getSize() == 3); }
}

TEST_CASE("Test json::Object") {
  std::map<std::string, std::unique_ptr<json::Node>> values;
  values["one"] = std::make_unique<json::Number>(1);
  values["two"] = std::make_unique<json::Number>(2);
  values["three"] = std::make_unique<json::Number>(3);
  json::Object object(std::move(values));

  SECTION("Test json::Object::getType") {
    REQUIRE(object.getType() == json::OBJECT);
  }

  SECTION("Test json::Object::to_string") {
    REQUIRE(object.to_string() == "{ one: 1, three: 3, two: 2 }");
  }

  SECTION("Test json::Object::operator[]") {
    REQUIRE(object["two"].getType() == json::NUMBER);
    REQUIRE_THROWS(object["four"]);
  }

  SECTION("Test json::Object::getSize") { REQUIRE(object.getSize() == 3); }
}

TEST_CASE("Test json::String") {
  json::String str("hello");

  SECTION("Test json::String::getType") {
    REQUIRE(str.getType() == json::STRING);
  }

  SECTION("Test json::String::to_string") {
    REQUIRE(str.to_string() == "\"hello\"");
  }

  SECTION("Test json::String::getValue") { REQUIRE(str.getValue() == "hello"); }
}

TEST_CASE("Test json::Number") {
  json::Number num(42);

  SECTION("Test json::Number::getType") {
    REQUIRE(num.getType() == json::NUMBER);
  }

  SECTION("Test json::Number::to_string") { REQUIRE(num.to_string() == "423"); }

  SECTION("Test json::Number::getValue") { REQUIRE(num.getValue() == 42); }
}

TEST_CASE("Test json::Constant") {
  json::Constant constant(json::TRUE);

  SECTION("Test json::Constant::getType") {
    REQUIRE(constant.getType() == json::CONSTANT);
  }

  SECTION("Test json::Constant::to_string") {
    REQUIRE(constant.to_string() == "true");
  }
}