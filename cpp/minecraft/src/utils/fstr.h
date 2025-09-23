// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file fstr.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 17:11
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef FSTR_H
#define FSTR_H
#pragma once

#include <array>

namespace minecraft {

    template<typename T, std::size_t N>
    struct FStr {
        std::array<T, N - 1> data;

        static constexpr std::size_t size = N - 1;

        constexpr FStr(const char (&value)[N]);

        template<std::size_t M>
        constexpr bool operator==(const FStr<T, M>& other) const;

        constexpr bool operator==(const FStr& other) const;

        template<std::size_t M>
        constexpr bool operator==(const T (&other)[M]) const;

        constexpr bool equals(const T* other, std::size_t len) const;
    };

    template<std::size_t N>
    using FStrChar = FStr<char, N>;

    template<std::size_t N>
    using FStrByte = FStr<std::byte, N>;

    template<typename>
    struct isFStr : std::false_type {};

    template<typename T, std::size_t N>
    struct isFStr<FStr<T, N>> : std::true_type {};

    template<typename T>
    inline constexpr bool isFStr_v = isFStr<T>::value;

    template<typename T>
    concept is_fstr = isFStr_v<T>;

    template<typename>
    struct isFStrChar : std::false_type {};

    template<std::size_t N>
    struct isFStrChar<FStrChar<N>> : std::true_type {};

    template<typename T>
    inline constexpr bool isFStrChar_v = isFStrChar<T>::value;

    template<typename T>
    concept is_fstr_char = isFStrChar_v<T>;

    template<typename>
    struct isFStrByte : std::false_type {};

    template<std::size_t N>
    struct isFStrByte<FStrByte<N>> : std::true_type {};

    template<typename T>
    inline constexpr bool isFStrByte_v = isFStrByte<T>::value;

    template<typename T>
    concept is_fstr_byte = isFStrByte_v<T>;

    template<FStrChar V>
    constexpr auto operator""_s();

    template<FStrByte V>
    constexpr auto operator""_b();

}  // namespace minecraft

#include "fstr.hpp"

#endif  // FSTR_H
