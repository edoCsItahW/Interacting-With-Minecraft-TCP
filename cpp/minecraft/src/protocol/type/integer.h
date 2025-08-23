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

            Integer(const Integer &other) = default;

            Integer(Integer &&other) = default;

            Integer &operator=(const Integer &other) = default;

            Integer &operator=(Integer &&other) = default;

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

            Integer() = default;

            Integer(const Integer &) = default;

            Integer(Integer &&) = default;

            Integer &operator=(const Integer &) = default;

            Integer &operator=(Integer &&) = default;

            constexpr auto serialize() const;

            static auto deserialize(const std::byte *data);

            static std::string toString();

            static std::string toHexString();
        };
    }  // namespace detail

    template<uint8_t... Vs>
    using Byte = detail::Integer<int8_t, Vs...>;

    using ByteType = Byte<>;

    template<typename>
    struct is_byte_field : std::false_type {};

    template<uint8_t... Vs>
    struct is_byte_field<Byte<Vs...>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_byte_type_v = is_byte_field<T>::value;

    template<typename T>
    concept byteField = is_byte_type_v<T>;

    template<int16_t... Vs>
    using Short = detail::Integer<int16_t, Vs...>;

    using ShortType = Short<>;

    template<typename>
    struct is_short_field : std::false_type {};

    template<int16_t... Vs>
    struct is_short_field<Short<Vs...>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_short_type_v = is_short_field<T>::value;

    template<typename T>
    concept shortField = is_short_type_v<T>;

    template<int32_t... Vs>
    using Int = detail::Integer<int32_t, Vs...>;

    using IntType = Int<>;

    template<typename>
    struct is_int_field : std::false_type {};

    template<int32_t... Vs>
    struct is_int_field<Int<Vs...>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_int_type_v = is_int_field<T>::value;

    template<typename T>
    concept intField = is_int_type_v<T>;

    template<int64_t... Vs>
    using Long = detail::Integer<int64_t, Vs...>;

    using LongType = Long<>;

    template<typename>
    struct is_long_field : std::false_type {};

    template<int64_t... Vs>
    struct is_long_field<Long<Vs...>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_long_type_v = is_long_field<T>::value;

    template<typename T>
    concept longField = is_long_type_v<T>;

    template<uint8_t... Vs>
    using UByte = detail::Integer<uint8_t, Vs...>;

    using UByteType = UByte<>;

    template<typename>
    struct is_ubyte_field : std::false_type {};

    template<uint8_t... Vs>
    struct is_ubyte_field<UByte<Vs...>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_ubyte_type_v = is_ubyte_field<T>::value;

    template<typename T>
    concept ubyteField = is_ubyte_type_v<T>;

    template<uint16_t... Vs>
    using UShort = detail::Integer<uint16_t, Vs...>;

    using UShortType = UShort<>;

    template<typename>
    struct is_ushort_field : std::false_type {};

    template<uint16_t... Vs>
    struct is_ushort_field<UShort<Vs...>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_ushort_type_v = is_ushort_field<T>::value;

    template<typename T>
    concept ushortField = is_ushort_type_v<T>;

    template<uint32_t... Vs>
    using UInt = detail::Integer<uint32_t, Vs...>;

    using UIntType = UInt<>;

    template<typename>
    struct is_uint_field : std::false_type {};

    template<uint32_t... Vs>
    struct is_uint_field<UInt<Vs...>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_uint_type_v = is_uint_field<T>::value;

    template<typename T>
    concept uintField = is_uint_type_v<T>;

    template<uint64_t... Vs>
    using ULong = detail::Integer<uint64_t, Vs...>;

    using ULongType = ULong<>;

    template<typename>
    struct is_ulong_field : std::false_type {};

    template<uint64_t... Vs>
    struct is_ulong_field<ULong<Vs...>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_ulong_type_v = is_ulong_field<T>::value;

    template<typename T>
    concept ulongField = is_ulong_type_v<T>;

    template<typename>
    struct is_integer_field : std::false_type {};

    template<typename T>
    struct is_integer_field<detail::Integer<T>> : std::true_type {};

    template<typename T, T V>
    struct is_integer_field<detail::Integer<T, V>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_integer_field_v = is_integer_field<T>::value;

    template<typename T>
    concept integerField = is_integer_field_v<T>;
}  // namespace minecraft::protocol

#include "integer.hpp"

#endif  // INTEGER_H
