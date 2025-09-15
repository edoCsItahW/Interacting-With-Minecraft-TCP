// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file fstr.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/15 14:24
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef FSTR_HPP
#define FSTR_HPP
#pragma once

namespace minecraft {

    template<typename T, std::size_t N>
    constexpr FStr<T, N>::FStr(const char (&value)[N]) {
        for (std::size_t i = 0; i < N - 1; ++i) data[i] = static_cast<T>(value[i]);
    }

    template<typename T, std::size_t N>
    template<std::size_t M>
    constexpr bool FStr<T, N>::operator==(const FStr<T, M>& other) const {
        if constexpr (N != M) return false;

        return std::equal(data.begin(), data.end(), other.data.begin());
    }

    template<typename T, std::size_t N>
    constexpr bool FStr<T, N>::operator==(const FStr& other) const {
        return size != other.size ? false : data == other.data;  // c++20
    }

    template<typename T, std::size_t N>
    template<std::size_t M>
    constexpr bool FStr<T, N>::operator==(const T (&other)[M]) const {
        if constexpr (N != M) return false;

        return std::equal(data.begin(), data.end(), other);
    }

    template<typename T, std::size_t N>
    constexpr bool FStr<T, N>::equals(const T* other, std::size_t len) const {
        if (len != size) return false;

        return std::equal(data.begin(), data.end(), other);
    }

    template<FStrChar V>
    constexpr auto operator""_s() {
        return V;
    }

    template<FStrByte V>
    constexpr auto operator""_b() {
        return V;
    }

}  // namespace minecraft

#endif  // FSTR_HPP
