//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MARIADBCLIENTPP_CONVERSION_HELPER_H
#define MARIADBCLIENTPP_CONVERSION_HELPER_H

#include <limits>
#include <stdexcept>
#include <charconv>
#include <utility>

#ifdef WIN32
#undef max
#undef min
#endif

#if _MSVC_LANG >= 201703 && !defined(__cpp_lib_to_chars)
#define __cpp_lib_to_chars
#endif

template <typename T, typename K>
inline T checked_cast(K value) {
    if (value < std::numeric_limits<T>::lowest() || value > std::numeric_limits<T>::max())
        return T();

    return static_cast<T>(value);
}

template <typename T>
inline T string_cast(const intercept::types::r_string &str) {
#ifdef __cpp_lib_to_chars
    int parsedNumber;
    auto [p, ec] = std::from_chars(str.data(), str.data() + str.size(), parsedNumber);

    if (p != str.data() + str.size()) return T();

    return checked_cast<T>(parsedNumber);
#else
    int parsedNumber = std::atoi(str.c_str());

    return checked_cast<T>(parsedNumber);
#endif
}

template <>
inline unsigned long string_cast(const intercept::types::r_string &str) {
#ifdef __cpp_lib_to_chars
    unsigned long parsedNumber;
    auto [p, ec] = std::from_chars(str.data(), str.data() + str.size(), parsedNumber);

    if (p != str.data() + str.size()) return 0;
#else
    char *endPos;
    unsigned long parsedNumber = std::strtoul(str.c_str(), &endPos, 10);

    if (endPos != str.end().operator const char *()) return 0;

#endif
    return parsedNumber;
}

template <>
inline unsigned int string_cast(const intercept::types::r_string &str) {
    unsigned long parsedNumber = string_cast<unsigned long>(str);

    return checked_cast<unsigned int>(parsedNumber);
}

template <>
inline unsigned long long string_cast(const intercept::types::r_string &str) {
    unsigned long long parsedNumber;
#ifdef __cpp_lib_to_chars
    auto [p, ec] = std::from_chars(str.data(), str.data() + str.size(), parsedNumber);

    if (p != str.data() + str.size()) return 0;
#else
    char *endPos;
    parsedNumber = std::strtoull(str.c_str(), &endPos, 10);

    if (endPos != str.end().operator const char *()) return 0;

#endif
    return parsedNumber;
}

template <>
inline long long string_cast(const intercept::types::r_string &str) {
    long long parsedNumber;
#ifdef __cpp_lib_to_chars
    auto [p, ec] = std::from_chars(str.data(), str.data() + str.size(), parsedNumber);

    if (p != str.data() + str.size()) return 0;
#else
    char *endPos;
    parsedNumber = std::strtoll(str.c_str(), &endPos, 10);

    if (endPos != str.end().operator const char *()) return 0;

#endif
    return parsedNumber;
}

template <>
inline double string_cast(const intercept::types::r_string &str) {
    double parsedNumber;
#ifdef __cpp_lib_to_chars
    auto [p, ec] = std::from_chars(str.data(), str.data() + str.size(), parsedNumber);

    if (ec == std::errc::result_out_of_range) {
        // Not a Number double
        return std::numeric_limits<double>::quiet_NaN();
    }

    if (p != str.data() + str.size()) return 0;
#else
    char *endPos;
    parsedNumber = std::strtod(str.c_str(), &endPos);

    if (endPos != str.end().operator const char *()) return 0;

#endif
    return parsedNumber;
}

template <>
inline float string_cast(const intercept::types::r_string &str) {
    float parsedNumber;
#ifdef __cpp_lib_to_chars
    auto [p, ec] = std::from_chars(str.data(), str.data() + str.size(), parsedNumber);

    if (ec == std::errc::result_out_of_range) {
        // Not a Number float
        return std::numeric_limits<float>::quiet_NaN();
    }

    if (p != str.data() + str.size()) return 0;
#else
    char *endPos;
    parsedNumber = std::strtof(str.c_str(), &endPos);

    if (endPos != str.end().operator const char *()) return 0;

#endif
    return parsedNumber;
}

#endif  // MARIADBCLIENTPP_CONVERSION_HELPER_H
