// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("VariantComparer") {
  SECTION("String") {
    Comparer<const char*> comparer("hello");

    SECTION("should return false for null") {
      comparer.visitNull();
      REQUIRE(comparer.result == false);
    }

    SECTION("should return true for matching string") {}
  }
}
