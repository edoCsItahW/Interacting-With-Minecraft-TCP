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

        template<typename T>
        struct Integer {
        private:
            mutable std::array<std::byte, sizeof(T)> data;

            mutable bool cached = false;

            static constexpr auto size_ = sizeof(T);

            T value_;

        public:
            using type = T;

            using serializeType = std::array<std::byte, size_>;

            Integer();

            explicit Integer(T value);

            Integer(const Integer &other) = default;

            Integer(Integer &&other) = default;

            Integer &operator=(const Integer &other) = default;

            Integer &operator=(Integer &&other) = default;

            [[nodiscard]] static constexpr std::size_t size();

            [[nodiscard]] type value() const;

            [[nodiscard]] serializeType serialize() const;

            static auto deserialize(const std::byte *data);

            [[nodiscard]] std::string toString() const;

            [[nodiscard]] std::string toHexString() const;
        };

    }  // namespace detail

    using Byte = detail::Integer<int8_t>;

    template<typename T>
    concept is_byte_field = std::is_same_v<T, Byte>;

    using Short = detail::Integer<int16_t>;

    template<typename T>
    concept is_short_field = std::is_same_v<T, Short>;

    using Int = detail::Integer<int32_t>;

    template<typename T>
    concept is_int_field = std::is_same_v<T, Int>;

    using Long = detail::Integer<int64_t>;

    template<typename T>
    concept is_long_field = std::is_same_v<T, Long>;

    using UByte = detail::Integer<uint8_t>;

    template<typename T>
    concept is_ubyte_field = std::is_same_v<T, UByte>;

    using UShort = detail::Integer<uint16_t>;

    template<typename T>
    concept is_ushort_field = std::is_same_v<T, UShort>;

    using UInt = detail::Integer<uint32_t>;

    template<typename T>
    concept is_uint_field = std::is_same_v<T, UInt>;

    using ULong = detail::Integer<uint64_t>;

    template<typename T>
    concept is_ulong_field = std::is_same_v<T, ULong>;

    template<typename>
    struct isIntegerField : std::false_type {};

    template<typename T>
    struct isIntegerField<detail::Integer<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool isIntegerField_v = isIntegerField<T>::value;

    template<typename T>
    concept is_integer_field = isIntegerField_v<T>;

}  // namespace minecraft::protocol

#include "integer.hpp"

#endif  // INTEGER_H
