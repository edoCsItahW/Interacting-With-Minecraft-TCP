// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file nullable.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/14 17:06
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef NULLABLE_HPP
#define NULLABLE_HPP
#pragma once

namespace minecraft {

    template<typename T>
    constexpr Nullable<T>::Nullable(const T& value)
        : value(value) {}

    template<typename T>
    constexpr bool Nullable<T>::hasValue() {
        return true;
    }

    template<typename T>
    constexpr T Nullable<T>::operator*() const {
        return value;
    }

    template<typename T>
    template<typename U>
    constexpr bool Nullable<T>::operator==(const Nullable<U>& other) const {
        if constexpr (std::is_same_v<T, U>) return value == other.value;

        return false;
    }

    constexpr bool Nullable<void>::hasValue() { return false; }

    template<typename T>
    constexpr bool Nullable<void>::operator==(const Nullable<T>& other) const {
        if constexpr (std::is_same_v<T, void>) return true;

        return false;
    }

}  // namespace minecraft

#endif  // NULLABLE_HPP
