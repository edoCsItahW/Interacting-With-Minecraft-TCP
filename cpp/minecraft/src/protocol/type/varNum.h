// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file varNum.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/16 12:42
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef VARNUM_H
#define VARNUM_H
#pragma once

#include <functional>
#include <string>

namespace minecraft::protocol {
    namespace detail {
        template<typename T>
        inline constexpr T SEGMENT_BITS{0x7F};

        template<typename T>
        inline constexpr T CONTINUE_BIT{0x80};

        template<typename T>
        concept intOrLong = std::is_same_v<T, int> || std::is_same_v<T, long>;

        template<typename T>
        constexpr std::size_t varNumSize(T value);

        template<intOrLong T, typename C>
        constexpr C varNumSerialize(T value, void(func)(C &, std::size_t, std::byte));

        template<intOrLong T>
        T varNumDeserialize(const std::byte *value);

        template<intOrLong T, auto...>
        struct VarNum;

        template<intOrLong T>
        struct VarNum<T> {
        private:
            mutable std::vector<std::byte> data;

        public:
            using type = T;

            T value;

            std::size_t size;

            VarNum();

            explicit VarNum(T value);

            auto serialize() const;

            static auto deserialize(const std::byte *data);

            [[nodiscard]] std::string toString() const;

            [[nodiscard]] std::string toHexString() const;
        };

        template<intOrLong T>
        VarNum(T) -> VarNum<T>;

        template<intOrLong T, T V>
        struct VarNum<T, V> {
            using type = T;

            static constexpr T value = V;

            static constexpr std::size_t size = varNumSize(value);

            static constexpr auto serialize();

            static auto deserialize(const std::byte *data);

            static std::string toString();

            static std::string toHexString();
        };
    }  // namespace detail

    template<int... Vs>
    using VarInt = detail::VarNum<int, Vs...>;

    using VarIntType = VarInt<>;

    template<long... Vs>
    using VarLong = detail::VarNum<long, Vs...>;

    using VarLongType = VarLong<>;

    template<typename>
    struct is_var_num_field : std::false_type {};

    template<detail::intOrLong T>
    struct is_var_num_field<detail::VarNum<T>> : std::true_type {};

    template<detail::intOrLong T, T V>
    struct is_var_num_field<detail::VarNum<T, V>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_var_num_field_v = is_var_num_field<T>::value;

    template<typename T>
    concept var_num_field = is_var_num_field_v<T>;

}  // namespace minecraft::protocol

#include "varNum.hpp"

#endif  // VARNUM_H
