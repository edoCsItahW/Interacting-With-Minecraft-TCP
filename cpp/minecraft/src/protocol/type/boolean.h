// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file boolean.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 20:35
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef BOOLEAN_H
#define BOOLEAN_H
#pragma once

#include <cstddef>
#include <string>
#include <array>

namespace minecraft::protocol {
    template<auto...>
    struct Boolean;

    template<>
    struct Boolean<> {
    private:
        mutable std::array<std::byte, 1> data{};

    public:
        using type = bool;

        static constexpr std::size_t size = 1;

        bool value;

        Boolean();

        Boolean(bool value);

        [[nodiscard]] auto serialize() const;

        static auto deserialize(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<bool V>
    struct Boolean<V> {
        using type = bool;

        static constexpr bool value = V;

        static constexpr std::size_t size = 1;

        static constexpr auto serialize();

        static auto deserialize(const std::byte* data);

        static std::string toString();

        static std::string toHexString();
    };

    using BooleanType = Boolean<>;

    template<typename>
    struct is_boolean_field : std::false_type {};

    template<>
    struct is_boolean_field<Boolean<>> : std::true_type {};

    template<bool V>
    struct is_boolean_field<Boolean<V>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_boolean_field_v = is_boolean_field<T>::value;

    template<typename T>
    concept boolean_field = is_boolean_field_v<T>;

}  // namespace minecraft::protocol

#include "boolean.hpp"

#endif  // BOOLEAN_H
