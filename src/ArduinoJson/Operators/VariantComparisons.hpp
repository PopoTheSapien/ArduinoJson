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
  bool result;
  T comparand;

  explicit Comparer(T value) : comparand(value) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *value) {
    result = adaptString(comparand).equals(value);
  }
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
  void visitNull() {}
};

template <typename T>
struct ArithmeticComparer {
  bool result;
  T comparand;

  explicit ArithmeticComparer(T value) : comparand(value) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float value) {
    result = value == comparand;
  }
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt value) {
    result = !(comparand + static_cast<T>(value));
  }
  void visitPositiveInteger(UInt value) {
    result = comparand == static_cast<T>(value);
  }
  void visitBoolean(bool) {}
  void visitNull() {}
};

template <typename T>
struct is_simple_value {
  static const bool value = is_integral<T>::value ||
                            is_floating_point<T>::value ||
                            is_same<T, bool>::value;
};

template <typename TVariant>
class VariantComparisons {
 public:
  // const char* == TVariant
  template <typename T>
  friend typename enable_if<IsString<T *>::value, bool>::type operator==(
      T *lhs, TVariant rhs) {
    Comparer<T *> comparer(lhs);
    rhs.accept(comparer);
    return comparer.result;
  }

  // std::string == TVariant
  template <typename T>
  friend typename enable_if<IsString<T>::value, bool>::type operator==(
      const T &lhs, TVariant rhs) {
    Comparer<const T &> comparer(lhs);
    rhs.accept(comparer);
    return comparer.result;
  }

  // TVariant == const char*
  template <typename T>
  friend typename enable_if<IsString<T *>::value, bool>::type operator==(
      TVariant lhs, T *rhs) {
    Comparer<T *> comparer(rhs);
    lhs.accept(comparer);
    return comparer.result;
  }

  // TVariant == std::string
  template <typename T>
  friend typename enable_if<IsString<T>::value, bool>::type operator==(
      TVariant lhs, const T &rhs) {
    Comparer<const T &> comparer(rhs);
    lhs.accept(comparer);
    return comparer.result;
  }

  // bool/int/float == TVariant
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator==(
      const T &lhs, TVariant rhs) {
    ArithmeticComparer<T> comparer(lhs);
    rhs.accept(comparer);
    return comparer.result;
  }

  // TVariant == bool/int/float
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator==(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() == rhs;
  }

  // const char* != TVariant
  template <typename T>
  friend typename enable_if<IsString<T *>::value, bool>::type operator!=(
      T *lhs, TVariant rhs) {
    return !operator==(lhs, rhs);
  }

  // std::string != TVariant
  template <typename T>
  friend typename enable_if<IsString<T>::value, bool>::type operator!=(
      const T &lhs, TVariant rhs) {
    return !operator==(lhs, rhs);
  }

  // TVariant != const char*
  template <typename T>
  friend typename enable_if<IsString<T *>::value, bool>::type operator!=(
      TVariant lhs, T *rhs) {
    return !operator==(lhs, rhs);
  }

  // TVariant != std::string
  template <typename T>
  friend typename enable_if<IsString<T>::value, bool>::type operator!=(
      TVariant lhs, const T &rhs) {
    return !operator==(lhs, rhs);
  }

  // bool/int/float != TVariant
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator!=(
      const T &lhs, TVariant rhs) {
    return lhs != rhs.template as<T>();
  }

  // TVariant != bool/int/float
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator!=(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() != rhs;
  }

  // bool/int/float < TVariant
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator<(
      const T &lhs, TVariant rhs) {
    return lhs < rhs.template as<T>();
  }

  // TVariant < bool/int/float
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator<(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() < rhs;
  }

  // bool/int/float <= TVariant
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator<=(
      const T &lhs, TVariant rhs) {
    return lhs <= rhs.template as<T>();
  }

  // TVariant <= bool/int/float
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator<=(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() <= rhs;
  }

  // bool/int/float > TVariant
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator>(
      const T &lhs, TVariant rhs) {
    return lhs > rhs.template as<T>();
  }

  // TVariant > bool/int/float
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator>(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() > rhs;
  }

  // bool/int/float >= TVariant
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator>=(
      const T &lhs, TVariant rhs) {
    return lhs >= rhs.template as<T>();
  }

  // TVariant >= bool/int/float
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator>=(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() >= rhs;
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
