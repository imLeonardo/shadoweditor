/* Copyright (C) 2022 Davide Faconti, Eurecat -  All Rights Reserved
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
*   to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
*   and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <type_traits>

#include "simple_string.hpp"

#undef max
#undef min

namespace SafeAny {

namespace details {

template<typename BoolCondition>
using EnableIf = typename std::enable_if<BoolCondition::value, void>::type;

template<typename T>
constexpr bool is_integer() {
    return std::is_same<T, bool>::value or std::is_same<T, char>::value or
           std::is_integral<T>::value;
}

template<typename T>
constexpr bool is_convertible_type() {
    return is_integer<T>() or std::is_floating_point<T>::value or
           std::is_same<T, std::string>::value or
           std::is_same<T, SimpleString>::value or std::is_enum<T>::value;
}

template<typename T>
constexpr bool is_convertible_to_bool() {
    return is_integer<T>() or std::is_floating_point<T>::value or
           std::is_enum<T>::value;
}

template<typename T>
constexpr bool is_signed() {
    return std::is_signed<T>::value;
}

template<typename T1, typename T2>
constexpr bool is_same() {
    return std::is_same<T1, T2>::value;
}

template<typename From, typename To>
inline void checkUpperLimit(const From &from) {
    if(from > static_cast<From>(std::numeric_limits<To>::max())) {
        throw std::runtime_error("Value outside the max numerical limit.");
    }
}

template<typename From, typename To>
inline void checkLowerLimit(const From &from) {
    if constexpr(std::is_same<To, bool>::value) {
        if(from != 0 and from != 1) {
            throw std::runtime_error("Implicit casting to bool is not allowed");
        }
    } else if(from < std::numeric_limits<To>::min()) {
        throw std::runtime_error("Value outside the lovest numerical limit.");
    }
}

template<typename From, typename To>
inline void checkTruncation(const From &from) {
    if(from != static_cast<From>(static_cast<To>(from))) {
        throw std::runtime_error("Floating point truncated");
    }
}

//----------------------- Implementation ----------------------------------------------

template<typename SRC, typename DST>
void convertNumber(const SRC &source, DST &target) {
    static_assert(
            is_convertible_type<SRC>() and is_convertible_type<DST>(),
            "Not "
            "convertible"
    );

    constexpr bool both_integers = is_integer<SRC>() and is_integer<DST>();

    if constexpr(is_signed<SRC>() and !is_signed<DST>()) {
        if(source < 0) {
            throw std::runtime_error(
                    "Value is negative and can't be converted to unsigned"
            );
        }
    }
    // these conversions are always safe:
    // - same type
    // - float -> double
    if constexpr(is_same<SRC, DST>() or
                 (is_same<SRC, float>() and is_same<DST, double>())) {
        // No check needed
        target = static_cast<DST>(source);
    } else if constexpr(both_integers) {
        if constexpr(sizeof(SRC) == sizeof(DST) and !is_signed<SRC>() and
                     is_signed<DST>()) {
            checkUpperLimit<SRC, DST>(source);
        }
        // casting to a smaller number need to be check
        else if constexpr(sizeof(SRC) > sizeof(DST)) {
            if constexpr(is_signed<SRC>()) {
                checkLowerLimit<SRC, DST>(source);
            }
            checkUpperLimit<SRC, DST>(source);
        }
        target = static_cast<DST>(source);
    }
    // special case: bool accept truncation
    else if constexpr(is_convertible_to_bool<SRC>() and is_same<DST, bool>()) {
        target = static_cast<DST>(source);
    }
    // casting to/from floating points might cause truncation.
    else if constexpr(std::is_floating_point<SRC>::value or
                      std::is_floating_point<DST>::value) {
        bool both_float = std::is_floating_point<SRC>::value and
                          std::is_floating_point<DST>::value;
        // to avoid being too pedantic, let's accept casting between double and float
        if(!both_float) {
            checkTruncation<SRC, DST>(source);
        }
        target = static_cast<DST>(source);
    }
}

}//end namespace details
}//end namespace SafeAny
