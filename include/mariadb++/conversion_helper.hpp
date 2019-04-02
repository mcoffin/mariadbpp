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

#ifdef WIN32
#undef max
#undef min
#endif

template <typename T, typename K>
inline T checked_cast(K value) {
    if (value < std::numeric_limits<T>::lowest() || value > std::numeric_limits<T>::max()) return T();

    return static_cast<T>(value);
}

template <typename T>
inline T string_cast(const intercept::types::r_string &str) {
    int parsedNumber;
    auto [p, ec] = std::from_chars(str.data(), str.data()+str.size(), parsedNumber);

    if (p != str.data()+str.size()) return T();

    return checked_cast<T>(parsedNumber);
}

template <>
inline unsigned long string_cast(const intercept::types::r_string &str) {
    unsigned long parsedNumber;
    auto [p, ec] = std::from_chars(str.data(), str.data()+str.size(), parsedNumber);

    if (p != str.data()+str.size()) return 0;

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
    auto [p, ec] = std::from_chars(str.data(), str.data()+str.size(), parsedNumber);

    if (p != str.data()+str.size()) return 0;

    return parsedNumber;
}

template <>
inline long long string_cast(const intercept::types::r_string &str) {
    long long parsedNumber;
    auto [p, ec] = std::from_chars(str.data(), str.data()+str.size(), parsedNumber);

    if (p != str.data()+str.size()) return 0;

    return parsedNumber;
}

template <>
inline double string_cast(const intercept::types::r_string &str) {
    double parsedNumber;
    auto [p, ec] = std::from_chars(str.data(), str.data()+str.size(), parsedNumber);

    if (ec ==  std::errc::result_out_of_range) {
        // Not a Number double
        return std::numeric_limits<double>::quiet_NaN();
    }

    if (p != str.data()+str.size()) return 0;

    return parsedNumber;
}

template <>
inline float string_cast(const intercept::types::r_string &str) {
    float parsedNumber;
    auto [p, ec] = std::from_chars(str.data(), str.data()+str.size(), parsedNumber);

    if (ec ==  std::errc::result_out_of_range) {
        // Not a Number float
        return std::numeric_limits<float>::quiet_NaN();
    }

    if (p != str.data()+str.size()) return 0;

    return parsedNumber;
}

#endif  // MARIADBCLIENTPP_CONVERSION_HELPER_H
