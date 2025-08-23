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

        constexpr FStr(const char (&value)[N]) {
            for (std::size_t i = 0; i < N - 1; ++i) data[i] = static_cast<T>(value[i]);
        }

        constexpr bool operator==(const FStr& other) const {
            return size != other.size ? false : data == other.data;  // c++20
        }

        template<std::size_t M>
        constexpr bool operator==(const T (&other)[M]) const {
            if constexpr ((std::is_same_v<T, char> && M != size + 1) || M != size)
                return false;

            return std::equal(data.begin(), data.end(), other);
        }

        constexpr bool equals(const T* other, std::size_t len) {
            if (len != size) return false;

            return std::equal(data.begin(), data.end(), other);
        }
    };

    template<std::size_t N>
    using FStrChar = FStr<char, N>;

    template<std::size_t N>
    using FStrByte = FStr<std::byte, N>;

    template<FStrChar V>
    constexpr auto operator""_s() {
        return V;
    }

    template<FStrByte V>
    constexpr auto operator""_b() {
        return V;
    }

}  // namespace minecraft

#endif  // FSTR_H
