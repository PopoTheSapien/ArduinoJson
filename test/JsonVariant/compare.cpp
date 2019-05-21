// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static const char* null = 0;

template <typename T>
void checkEquals(T a, T b) {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();
  variant.set(a);

  REQUIRE(b == variant);
  REQUIRE(variant == b);
  REQUIRE(b <= variant);
  REQUIRE(variant <= b);
  REQUIRE(b >= variant);
  REQUIRE(variant >= b);

  REQUIRE_FALSE(b != variant);
  REQUIRE_FALSE(variant != b);
  REQUIRE_FALSE(b > variant);
  REQUIRE_FALSE(variant > b);
  REQUIRE_FALSE(b < variant);
  REQUIRE_FALSE(variant < b);
}

template <typename T>
void checkGreater(T a, T b) {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();
  variant.set(a);

  REQUIRE(variant > b);
  REQUIRE(b < variant);
  REQUIRE(variant != b);
  REQUIRE(b != variant);

  REQUIRE_FALSE(variant < b);
  REQUIRE_FALSE(b > variant);
  REQUIRE_FALSE(variant == b);
  REQUIRE_FALSE(b == variant);
}

template <typename T>
void checkLower(T a, T b) {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();
  variant.set(a);

  REQUIRE(variant < b);
  REQUIRE(b > variant);
  REQUIRE(variant != b);
  REQUIRE(b != variant);

  REQUIRE_FALSE(variant > b);
  REQUIRE_FALSE(b < variant);
  REQUIRE_FALSE(variant == b);
  REQUIRE_FALSE(b == variant);
}

template <typename T>
void checkComparisons(T low, T mid, T high) {
  checkEquals(mid, mid);
  checkGreater(mid, low);
  checkLower(mid, high);
}

TEST_CASE("JsonVariant comparisons") {
  SECTION("Double") {
    checkComparisons<double>(123.44, 123.45, 123.46);
  }

  SECTION("Float") {
    checkComparisons<float>(123.44f, 123.45f, 123.46f);
  }

  SECTION("SChar") {
    checkComparisons<signed char>(122, 123, 124);
  }

  SECTION("SInt") {
    checkComparisons<signed int>(122, 123, 124);
  }

  SECTION("SLong") {
    checkComparisons<signed long>(122L, 123L, 124L);
  }

  SECTION("SShort") {
    checkComparisons<signed short>(122, 123, 124);
  }

  SECTION("UChar") {
    checkComparisons<unsigned char>(122, 123, 124);
  }

  SECTION("UInt") {
    checkComparisons<unsigned int>(122, 123, 124);
  }

  SECTION("ULong") {
    checkComparisons<unsigned long>(122L, 123L, 124L);
  }

  SECTION("UShort") {
    checkComparisons<unsigned short>(122, 123, 124);
  }

  SECTION("null") {
    DynamicJsonDocument doc(4096);
    JsonVariant variant = doc.to<JsonVariant>();
    variant.set(null);

    REQUIRE(variant == variant);
    REQUIRE_FALSE(variant != variant);

    REQUIRE(variant == null);
    REQUIRE_FALSE(variant != null);

    REQUIRE(variant != "null");
    REQUIRE_FALSE(variant == "null");
  }

  SECTION("StringLiteral") {
    DynamicJsonDocument doc(4096);
    deserializeJson(doc, "\"hello\"");
    JsonVariant variant = doc.as<JsonVariant>();

    REQUIRE(variant == variant);
    REQUIRE_FALSE(variant != variant);

    REQUIRE(variant == "hello");
    REQUIRE_FALSE(variant != "hello");

    REQUIRE(variant != "world");
    REQUIRE_FALSE(variant == "world");

    REQUIRE(variant != null);
    REQUIRE_FALSE(variant == null);

    REQUIRE("hello" == variant);
    REQUIRE_FALSE("hello" != variant);

    REQUIRE("world" != variant);
    REQUIRE_FALSE("world" == variant);

    REQUIRE(null != variant);
    REQUIRE_FALSE(null == variant);
  }

  SECTION("String") {
    DynamicJsonDocument doc(4096);
    JsonVariant variant = doc.to<JsonVariant>();
    variant.set("hello");

    REQUIRE(variant == variant);
    REQUIRE_FALSE(variant != variant);

    REQUIRE(variant == std::string("hello"));
    REQUIRE_FALSE(variant != std::string("hello"));

    REQUIRE(variant != std::string("world"));
    REQUIRE_FALSE(variant == std::string("world"));

    REQUIRE(variant != null);
    REQUIRE_FALSE(variant == null);

    REQUIRE(std::string("hello") == variant);
    REQUIRE_FALSE(std::string("hello") != variant);

    REQUIRE(std::string("world") != variant);
    REQUIRE_FALSE(std::string("world") == variant);

    REQUIRE(null != variant);
    REQUIRE_FALSE(null == variant);
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("VLA equals") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonDocument doc(4096);
    JsonVariant variant = doc.to<JsonVariant>();
    variant.set("hello");

    REQUIRE((vla == variant));
    REQUIRE((variant == vla));
    REQUIRE_FALSE((vla != variant));
    REQUIRE_FALSE((variant != vla));
  }

  SECTION("VLA differs") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonDocument doc(4096);
    JsonVariant variant = doc.to<JsonVariant>();
    variant.set("world");

    REQUIRE((vla != variant));
    REQUIRE((variant != vla));
    REQUIRE_FALSE((vla == variant));
    REQUIRE_FALSE((variant == vla));
  }
#endif

  DynamicJsonDocument doc1(4096), doc2(4096), doc3(4096);
  JsonVariant variant1 = doc1.to<JsonVariant>();
  JsonVariant variant2 = doc2.to<JsonVariant>();
  JsonVariant variant3 = doc3.to<JsonVariant>();

  SECTION("Variants containing integers") {
    variant1.set(42);
    variant2.set(42);
    variant3.set(666);

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("Variants containing linked strings") {
    // create two identical strings at different addresses
    char hello1[] = "hello";
    char hello2[] = "hello";
    REQUIRE(hello1 != hello2);

    variant1.set(hello1);
    variant2.set(hello2);
    variant3.set("world");

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("Variants containing owned strings") {
    variant1.set(std::string("hello"));
    variant2.set(std::string("hello"));
    variant3.set(std::string("world"));

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("Variants containing linked raws") {
    // create two identical strings at different addresses
    char hello1[] = "hello";
    char hello2[] = "hello";
    REQUIRE(hello1 != hello2);

    variant1.set(serialized(hello1));
    variant2.set(serialized(hello2));
    variant3.set(serialized("world"));

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("Variants containing owned raws") {
    variant1.set(serialized(std::string("hello")));
    variant2.set(serialized(std::string("hello")));
    variant3.set(serialized(std::string("world")));

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("Variants containing double") {
    variant1.set(42.0);
    variant2.set(42.0);
    variant3.set(666.0);

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("BoolInVariant") {
    variant1.set(true);
    variant2.set(true);
    variant3.set(false);

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("ArrayInVariant") {
    JsonArray array1 = variant1.to<JsonArray>();
    JsonArray array2 = variant2.to<JsonArray>();

    array1.add(42);
    array2.add(42);

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("ObjectInVariant") {
    JsonObject obj1 = variant1.to<JsonObject>();
    JsonObject obj2 = variant2.to<JsonObject>();

    obj1["hello"] = "world";
    obj2["hello"] = "world";

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("Variant contains a string") {
    variant1.set("hello");

    SECTION("Compare with an integer") {
      REQUIRE(variant1 != 42);
      REQUIRE_FALSE(variant1 == 42);

      REQUIRE(42 != variant1);
      REQUIRE_FALSE(42 == variant1);
    }

    SECTION("Compare with NULL") {
      REQUIRE(variant1 != NULL);
      REQUIRE_FALSE(variant1 == NULL);

      REQUIRE(NULL != variant1);
      REQUIRE_FALSE(NULL == variant1);
    }

    SECTION("Compare with a different string") {
      REQUIRE(variant1 != "world");
      REQUIRE_FALSE(variant1 == "world");

      REQUIRE("world" != variant1);
      REQUIRE_FALSE("world" == variant1);
    }

    SECTION("Compare with an identical string") {
      REQUIRE(variant1 == "hello");
      REQUIRE_FALSE(variant1 != "hello");

      REQUIRE("hello" == variant1);
      REQUIRE_FALSE("hello" != variant1);
    }
  }

  SECTION("Variant contains null") {
    variant1.clear();

    SECTION("Compare with an integer") {
      REQUIRE(variant1 != 42);
      REQUIRE_FALSE(variant1 == 42);

      REQUIRE(42 != variant1);
      REQUIRE_FALSE(42 == variant1);
    }

    // SECTION("Compare with NULL") {
    //   REQUIRE(variant1 == NULL);
    //   REQUIRE_FALSE(variant1 != NULL);

    //   REQUIRE(NULL == variant1);
    //   REQUIRE_FALSE(NULL != variant1);
    // }

    SECTION("Compare with a string") {
      REQUIRE(variant1 != "world");
      REQUIRE_FALSE(variant1 == "world");

      REQUIRE("world" != variant1);
      REQUIRE_FALSE("world" == variant1);
    }
  }

  SECTION("Variant contains a positive integer") {
    variant1.set(42);

    SECTION("Compare with the same integer") {
      SECTION("Variant on the left") {
        REQUIRE(variant1 == 42);
        REQUIRE_FALSE(variant1 != 42);

        REQUIRE(variant1 <= 42);
        REQUIRE_FALSE(variant1 > 42);

        REQUIRE(variant1 >= 42);
        REQUIRE_FALSE(variant1 < 42);
      }

      SECTION("Variant on the right") {
        REQUIRE(42 == variant1);
        REQUIRE_FALSE(42 != variant1);

        REQUIRE(42 <= variant1);
        REQUIRE_FALSE(42 > variant1);

        REQUIRE(42 >= variant1);
        REQUIRE_FALSE(42 < variant1);
      }
    }

    SECTION("Compare with a larger integer") {
      SECTION("Variant on the left") {
        REQUIRE_FALSE(variant1 == 43);
        REQUIRE(variant1 != 43);

        REQUIRE(variant1 <= 43);
        REQUIRE_FALSE(variant1 > 43);

        REQUIRE_FALSE(variant1 >= 43);
        REQUIRE(variant1 < 43);
      }

      SECTION("Variant on the right") {
        REQUIRE_FALSE(43 == variant1);
        REQUIRE(43 != variant1);

        REQUIRE_FALSE(43 <= variant1);
        REQUIRE(43 > variant1);

        REQUIRE(43 >= variant1);
        REQUIRE_FALSE(43 < variant1);
      }
    }

    SECTION("Compare with a smaller integer") {
      SECTION("Variant on the left") {
        REQUIRE_FALSE(variant1 == 41);
        REQUIRE(variant1 != 41);

        REQUIRE_FALSE(variant1 <= 41);
        REQUIRE(variant1 > 41);

        REQUIRE(variant1 >= 41);
        REQUIRE_FALSE(variant1 < 41);
      }

      SECTION("Variant on the right") {
        REQUIRE_FALSE(41 == variant1);
        REQUIRE(41 != variant1);

        REQUIRE(41 <= variant1);
        REQUIRE_FALSE(41 > variant1);

        REQUIRE(43 >= variant1);
        REQUIRE_FALSE(43 < variant1);
      }
    }
  }
  // TODO: test variant containing positive and negative integers
}
