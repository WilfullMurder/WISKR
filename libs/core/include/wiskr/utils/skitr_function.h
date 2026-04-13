#ifndef WISKR_SKITR_FUNCTION_H
#define WISKR_SKITR_FUNCTION_H

#include <wiskr/wiskr_core_export.h>

#include <skitr/core/value.h>
#include <skitr/registry/method_registry.h>

#include <functional>
#include <vector>
#include <string>
#include <type_traits>
#include <stdexcept>
#include <map>

namespace whisker {
    namespace utils {
        using Value = skitter::core::Value;

        /**
         * @brief Metafunction that tests whether a type `T` is a specialization of a template `Template`.
         *
         * Example:
         * @code
         * is_specialization<std::vector<int>, std::vector>::value // true
         * @endcode
         */
        template<class T, template<class...> class Template>
        struct is_specialization : std::false_type {};

        template<template<class...> class Template, class... Args>
        struct is_specialization<Template<Args...>, Template> : std::true_type {};

        /**
         * @brief Convert a `Value` to type `T`.
         *
         * Primary template is declared here and specializations for supported types
         * are provided below. On type mismatch implementations throw
         * `std::runtime_error`.
         *
         * @tparam T Target type to convert to.
         * @param val Input `Value`.
         * @return Converted value of type `T`.
         */
        template<typename T>
        T value_cast(const Value& val);

        /**
         * @brief Convert to boolean.
         * @throws std::runtime_error if `val` is not a boolean.
         */
        template<>
        inline bool value_cast<bool>(const Value& val) {
            if (!val.isBool()) throw std::runtime_error("Value is not a bool");
            return val.asBool();
        }

        /**
         * @brief Convert to int.
         * @throws std::runtime_error if `val` is not an integer.
         */
        template<>
        inline int value_cast<int>(const Value& val) {
            if (!val.isInt()) throw std::runtime_error("Value is not an int");
            return val.asInt();
        }

        /**
         * @brief Convert to double.
         * @throws std::runtime_error if `val` is not a double.
         */
        template<>
        inline double value_cast<double>(const Value& val) {
            if (!val.isDouble()) throw std::runtime_error("Value is not a double");
            return val.asDouble();
        }

        /**
         * @brief Convert to std::string.
         * @throws std::runtime_error if `val` is not a string.
         */
        template<>
        inline std::string value_cast<std::string>(const Value& val) {
            if (!val.isString()) throw std::runtime_error("Value is not a string");
            return val.asString();
        }

        /**
         * @brief Convert to the underlying array type of `Value`.
         * @throws std::runtime_error if `val` is not an array.
         */
        template<>
        inline Value::array_t value_cast<Value::array_t>(const Value& val) {
            if (!val.isArray()) throw std::runtime_error("Value is not an array");
            return val.asArray();
        }

        /**
         * @brief Convert to a map representing an object `Value`.
         * @throws std::runtime_error if `val` is not an object.
         */
        template<>
        inline  std::map<std::string, Value> value_cast< std::map<std::string, Value>>(const Value& val) {
            if (!val.isObject()) throw std::runtime_error("Value is not an object");
            return val.asObject();
        }

        /**
         * @brief Convert an array `Value` to a `std::vector<T>`.
         *
         * Each element of the `Value` array is converted to `T` by recursively
         * calling `value_cast<T>`. Throws `std::runtime_error` if the input value
         * is not an array or if any element fails to convert.
         *
         * @tparam T Element type of the resulting vector.
         * @param val Input `Value` expected to be an array.
         * @return std::vector<T> Converted vector of elements.
         */
        template<typename T>
        inline std::vector<T> value_cast(const Value& val) {
            if (!val.isArray()) throw std::runtime_error("Value is not an array");

            std::vector<T> result;
            const auto& arr = val.asArray();
            result.reserve(arr.size());

            for (const auto& item : arr) {
                result.push_back(value_cast<T>(item));
            }
            return result;
        }

        /**
         * @brief Convert a `Value` to `std::optional<T>`.
         * @tparam T  Target type to convert to.
         * @param val  Input `Value`. If `val` is null, returns `std::nullopt`. Otherwise attempts to convert to `T` using `value_cast<T>`.
         * @return std::optional<T> Converted value wrapped in `std::optional`, or `std::nullopt` if input is null.
         */
        template<typename T>
        inline std::optional<T> value_cast_optional(const Value& val) {
            if (val.isNull()) return std::nullopt;
            return value_cast<T>(val);
        }


    } // namespace utils
} // namespace whisker

#endif //WISKR_SKITR_FUNCTION_H
