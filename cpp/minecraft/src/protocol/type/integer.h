// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file integer.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/17 10:51
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef INTEGER_H
#define INTEGER_H
#pragma once

#include <array>
#include <string>

namespace minecraft::protocol {
    namespace detail {
        template<typename T>
        concept integer = std::is_integral_v<T>;

        template<typename T, std::size_t N>
        constexpr std::array<std::byte, N> intSerialize(T value);

        template<typename T>
        T intDeserialize(const std::byte *data);

        template<typename, auto...>
        struct Integer;

        template<typename T>
        struct Integer<T> {
        private:
            mutable std::array<std::byte, sizeof(T)> data;

            mutable bool cached = false;

        public:
            using type = T;

            static constexpr auto size = sizeof(T);

            T value;

            Integer();

            explicit Integer(T value);

            auto serialize() const;

            static auto deserialize(const std::byte *data);

            [[nodiscard]] std::string toString() const;

            [[nodiscard]] std::string toHexString() const;
        };

        template<typename T>
        Integer(T) -> Integer<T>;

        template<typename T, T V>
        struct Integer<T, V> {
            using type = T;

            static constexpr auto size = sizeof(T);

            static constexpr T value = V;

            constexpr auto serialize() const;

            static auto deserialize(const std::byte *data);

            static std::string toString();

            static std::string toHexString();
        };
    }  // namespace detail

    template<uint8_t... Vs>
    using Byte = detail::Integer<int8_t, Vs...>;

    using ByteType = Byte<>;

    template<int16_t... Vs>
    using Short = detail::Integer<int16_t, Vs...>;

    using ShortType = Short<>;

    template<int32_t... Vs>
    using Int = detail::Integer<int32_t, Vs...>;

    using IntType = Int<>;

    template<int64_t... Vs>
    using Long = detail::Integer<int64_t, Vs...>;

    using LongType = Long<>;

    template<uint8_t... Vs>
    using UByte = detail::Integer<uint8_t, Vs...>;

    using UByteType = UByte<>;

    template<uint16_t... Vs>
    using UShort = detail::Integer<uint16_t, Vs...>;

    using UShortType = UShort<>;

    template<uint32_t... Vs>
    using UInt = detail::Integer<uint32_t, Vs...>;

    using UIntType = UInt<>;

    template<uint64_t... Vs>
    using ULong = detail::Integer<uint64_t, Vs...>;

    using ULongType = ULong<>;

    template<typename>
    struct is_integer_field : std::false_type {};

    template<typename T>
    struct is_integer_field<detail::Integer<T>> : std::true_type {};

    template<typename T, T V>
    struct is_integer_field<detail::Integer<T, V>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_integer_field_v = is_integer_field<T>::value;

    template<typename T>
    concept integer_field = is_integer_field_v<T>;
}  // namespace minecraft::protocol

#include "integer.hpp"

#endif  // INTEGER_H
