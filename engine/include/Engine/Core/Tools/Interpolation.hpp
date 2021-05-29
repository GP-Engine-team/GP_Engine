/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <GPM/constants.hpp>

/**
 * @see : https://easings.net/
 */
namespace GPE
{
template <typename T>
inline constexpr T half_one = static_cast<T>(0.5);
template <typename T>
inline constexpr T zero = static_cast<T>(0);
template <typename T>
inline constexpr T one = static_cast<T>(1);
template <typename T>
inline constexpr T two = static_cast<T>(2);
template <typename T>
inline constexpr T three = static_cast<T>(3);
template <typename T>
inline constexpr T four = static_cast<T>(4);
template <typename T>
inline constexpr T eighths = static_cast<T>(8);
template <typename T>
inline constexpr T ten = static_cast<T>(10);

/**
 * @brief easeInSine interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeInSine(T x)
{
    return one<T> - std::cos((x * PI) / one<T>);
}

/**
 * @brief easeOutSine interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeOutSine(T x)
{
    return std::sin((x * PI) / two<T>);
}

/**
 * @brief easeInOutSine interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeInOutSine(T x)
{
    return -(std::cos(PI * x) - one<T>) / two<T>;
}

/**
 * @brief easeIn interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <size_t Pow = 2, typename T>
inline constexpr T easeIn(T x)
{
    return std::pow(x, Pow);
}

/**
 * @brief easeOut interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <size_t Pow = 2, typename T>
inline constexpr T easeOut(T x)
{
    return one<T> - std::pow(one<T> - x, Pow);
}

/**
 * @brief easeInOut interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <size_t Pow = 2, typename T>
inline constexpr T easeInOut(T x)
{
    return x < half_one<T> ? std::pow(two<T>, Pow - 1) * std::pow(x, Pow)
                           : one<T> - std::pow(-two<T> * x + two<T>, Pow) / two<T>;
}

/**
 * @brief easeInCirc interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeInCirc(T x)
{
    return one<T> - std::sqrt(one<T> - std::pow(x, 2));
}

/**
 * @brief easeInCirc interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeOutCirc(T x)
{
    return std::sqrt(one<T> - std::pow(x - one<T>, 2));
}

/**
 * @brief easeInBack interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeInBack(T x)
{
    const T c1 = static_cast<T>(1.70158);
    const T c3 = c1 + one<T>;

    return c3 * x * x * x - c1 * x * x;
}

/**
 * @brief easeOutBack interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeOutBack(T x)
{
    const T c1 = static_cast<T>(1.70158);
    const T c3 = c1 + one<T>;

    return one<T> + c3 * pow(x - one<T>, 3) + c1 * pow(x - one<T>, 2);
}

/**
 * @brief easeInOutBack interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeInOutBack(T x)
{
    const T c1 = static_cast<T>(1.70158);
    const T c2 = c1 * static_cast<T>(1.525);

    return x < half_one<T>
               ? (std::pow(two<T> * x, 2) * ((c2 + one<T>)*two<T> * x - c2)) / two<T>
               : (std::pow(two<T> * x - two<T>, 2) * ((c2 + one<T>)*(x * two<T> - two<T>)+c2) + two<T>) / two<T>;
}

/**
 * @brief easeInElastic interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeInElastic(T x)
{
    const T c4 = (two<T> * PI) / three<T>;

    return x == zero<T>  ? zero<T>
           : x == one<T> ? one<T>
                         : -std::pow(two<T>, ten<T> * x - ten<T>) * std::sin((x * ten<T> - static_cast<T>(10.75)) * c4);
}

/**
 * @brief easeOutElastic interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeOutElastic(T x)
{
    const T c4 = (two<T> * PI) / three<T>;

    return x == zero<T>  ? zero<T>
           : x == one<T> ? one<T>
                         : std::pow(two<T>, -ten<T> * x) * sin((x * ten<T> - static_cast<T>(0.75)) * c4) + one<T>;
}

/**
 * @brief easeInOutElastic interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeInOutElastic(T x)
{
    const T c5 = (two<T> * PI) / static_cast<T>(4.5);

    return x == zero<T>      ? zero<T>
           : x == one<T>     ? one<T>
           : x < half_one<T> ? -(std::pow(two<T>, static_cast<T>(20) * x - ten<T>) *
                                 std::sin((static_cast<T>(20) * x - static_cast<T>(11.125)) * c5)) /
                                   two<T>
                             : (std::pow(two<T>, -static_cast<T>(20) * x + ten<T>) *
                                std::sin((static_cast<T>(20) * x - static_cast<T>(11.125)) * c5)) /
                                       two<T> +
                                   one<T>;
}

/**
 * @brief easeInBounce interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeInBounce(T x)
{
    return one<T> - easeOutBounce(one<T> - x);
}

/**
 * @brief easeOutBounce interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeOutBounce(T x)
{
    const T n1 = static_cast<T>(7.5625);
    const T d1 = static_cast<T>(2.75);

    if (x < one<T> / d1)
    {
        return n1 * x * x;
    }
    else if (x < two<T> / d1)
    {
        return n1 * (x -= static_cast<T>(1.5) / d1) * x + static_cast<T>(0.75);
    }
    else if (x < static_cast<T>(2.5) / d1)
    {
        return n1 * (x -= static_cast<T>(2.25) / d1) * x + static_cast<T>(0.9375);
    }
    else
    {
        return n1 * (x -= static_cast<T>(2.625) / d1) * x + static_cast<T>(0.984375);
    }
}

/**
 * @brief easeInOutBounce interpolation
 * @tparam T : floating type of the variable
 * @param x : [0, 1] or undifine behaviour
 * @return image of interpolation in x
 */
template <typename T>
inline constexpr T easeInOutBounce(T x)
{
    return x < half_one<T> ? (one<T> - easeOutBounce(one<T> - two<T> * x)) / two<T>
                           : (one<T> + easeOutBounce(two<T> * x - one<T>)) / two<T>;
}

} // namespace GPE
