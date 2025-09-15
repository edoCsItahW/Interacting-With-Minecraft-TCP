// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file nullable.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/14 17:02
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef NULLABLE_H
#define NULLABLE_H
#pragma once

#include <type_traits>

namespace minecraft {

    template<typename T>
    struct Nullable {
        T value;

        using type = T;

        Nullable() = delete;

        constexpr Nullable(const T& value);

        static constexpr bool hasValue();

        [[nodiscard]] constexpr T operator*() const;

        template<typename U>
        constexpr bool operator==(const Nullable<U>& other) const;
    };

    template<>
    struct Nullable<void> {
        using type = void;

        Nullable() = default;

        [[nodiscard]] static constexpr bool hasValue();

        template<typename T>
        constexpr bool operator==(const Nullable<T>& other) const;
    };

    using NullType = Nullable<void>;

    inline constexpr auto Null = NullType{};

    template<typename>
    struct isNullable : std::false_type {};

    template<typename T>
    struct isNullable<Nullable<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool isNullable_v = isNullable<T>::value;

    template<typename T>
    concept is_nullable = isNullable_v<T>;

}  // namespace minecraft

#include "nullable.hpp"

#endif  // NULLABLE_H
