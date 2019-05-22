// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Variant/VariantRef.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename T, typename Enable = void>
struct Comparer;

template <typename T>
struct Comparer<T, typename enable_if<IsString<T>::value>::type> {
  T lhs;
  int8_t result;

  explicit Comparer(T value) : lhs(value), result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *rhs) {
    result = adaptString(lhs).compare(rhs);
  }
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
  void visitNull() {
    result = adaptString(lhs).compare(NULL);
  }
};

template <typename T>
int8_t sign(const T &value) {
  return value < 0 ? -1 : value > 0 ? 1 : 0;
}

template <typename T>
struct Comparer<T, typename enable_if<is_integral<T>::value ||
                                      is_floating_point<T>::value>::type> {
  T lhs;
  int8_t result;

  explicit Comparer(T value) : lhs(value), result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float rhs) {
    result = sign(rhs - lhs);
  }
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt rhs) {
    result = sign(-static_cast<T>(rhs) - lhs);
  }
  void visitPositiveInteger(UInt rhs) {
    result = static_cast<T>(rhs) < lhs ? -1 : static_cast<T>(rhs) > lhs ? 1 : 0;
  }
  void visitBoolean(bool) {}
  void visitNull() {}
};

template <>
struct Comparer<bool, void> {
  bool lhs;
  int8_t result;

  explicit Comparer(bool value) : lhs(value), result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool rhs) {
    result = static_cast<int8_t>(rhs - lhs);
  }
  void visitNull() {}
};

template <typename TVariant>
class VariantComparisons {
 private:
  template <typename T>
  static int8_t compare(TVariant lhs, const T &rhs) {
    Comparer<T> comparer(rhs);
    lhs.accept(comparer);
    return comparer.result;
  }

 public:
  // const char* == TVariant
  template <typename T>
  friend bool operator==(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) == 0;
  }

  // string/int/float == TVariant
  template <typename T>
  friend bool operator==(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) == 0;
  }

  // TVariant == const char*
  template <typename T>
  friend bool operator==(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) == 0;
  }

  // TVariant == string/int/float
  template <typename T>
  friend bool operator==(TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) == 0;
  }

  // const char* != TVariant
  template <typename T>
  friend bool operator!=(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) != 0;
  }

  // string/int/float != TVariant
  template <typename T>
  friend bool operator!=(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) != 0;
  }

  // TVariant != const char*
  template <typename T>
  friend bool operator!=(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) != 0;
  }

  // TVariant != string/int/float
  template <typename T>
  friend bool operator!=(TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) != 0;
  }

  // bool/int/float < TVariant
  template <typename T>
  friend bool operator<(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) > 0;
  }

  // TVariant < bool/int/float
  template <typename T>
  friend bool operator<(TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) < 0;
  }

  // bool/int/float <= TVariant
  template <typename T>
  friend bool operator<=(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) >= 0;
  }

  // TVariant <= bool/int/float
  template <typename T>
  friend bool operator<=(TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) <= 0;
  }

  // bool/int/float > TVariant
  template <typename T>
  friend bool operator>(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) < 0;
  }

  // TVariant > bool/int/float
  template <typename T>
  friend bool operator>(TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) > 0;
  }

  // bool/int/float >= TVariant
  template <typename T>
  friend bool operator>=(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) <= 0;
  }

  // TVariant >= bool/int/float
  template <typename T>
  friend bool operator>=(TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) >= 0;
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
