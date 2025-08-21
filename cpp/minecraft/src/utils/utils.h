// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file utils.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/16 14:40
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef UTILS_H
#define UTILS_H
#pragma once

#include "fstr.h"
#include <array>
#include <string>
#include <type_traits>

namespace minecraft {
    template<typename T>
    concept numeric = std::is_arithmetic_v<T>;

    namespace detail {
        template<typename T, T V, int N = 1>
        inline constexpr T binpow_impl = N == 0     ? 1
                                       : N % 2 == 0 ? binpow_impl<T, V, N / 2> * binpow_impl<T, V, N / 2>
                                                    : binpow_impl<T, V, N / 2> * binpow_impl<T, V, N / 2> * V;

        template<typename T, std::size_t N = 0>
        constexpr auto enumMax();

    }  // namespace detail

    template<numeric auto V, int N = 1>
    inline constexpr decltype(V) binpow_v = detail::binpow_impl<decltype(V), V, N>;

    template<typename T>
    constexpr T binpow(T base, int exp);

    template<typename T>
    std::string toHexString(const T& value);

    template<std::size_t N>
    constexpr auto toHexArray(const std::array<uint8_t, N>& digest);

    template<typename T>
        requires requires(T t) { t.size(); }
    constexpr auto strToBytes(const T& str);

    constexpr auto strToBytes(const char* str);

    template<FStr V>
    constexpr auto strToBytes();

    constexpr std::size_t strLen(const char* str);

    template<auto V>
    constexpr auto enumToStr();

    template<typename T>
        requires std::is_enum_v<T>
    constexpr auto enumToStr(T value);

}  // namespace minecraft

#include "utils.hpp"

#endif  // UTILS_H
