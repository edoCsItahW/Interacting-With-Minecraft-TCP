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

        template<intOrLong T>
        struct VarNum {
        private:
            mutable std::vector<std::byte> data;

            std::size_t size_;

            T value_;

        public:
            using type = T;

            using serializeType = std::vector<std::byte>;

            VarNum();

            explicit VarNum(T value);

            VarNum(const VarNum &other) = default;

            VarNum(VarNum &&other) = default;

            VarNum &operator=(const VarNum &other) = default;

            VarNum &operator=(VarNum &&other) = default;

            [[nodiscard]] std::size_t size() const;

            [[nodiscard]] type value() const;

            [[nodiscard]] serializeType serialize() const;

            static auto deserialize(const std::byte *data);

            [[nodiscard]] std::string toString() const;

            [[nodiscard]] std::string toHexString() const;
        };

    }  // namespace detail

    using VarInt = detail::VarNum<int>;

    template<typename T>
    concept is_var_int_field = std::is_same_v<T, VarInt>;

    using VarLong = detail::VarNum<long>;

    template<typename T>
    concept is_var_long_field = std::is_same_v<T, VarLong>;

    template<typename>
    struct isVarNumField : std::false_type {};

    template<detail::intOrLong T>
    struct isVarNumField<detail::VarNum<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool isVarNumField_v = isVarNumField<T>::value;

    template<typename T>
    concept is_var_num_field = isVarNumField_v<T>;

}  // namespace minecraft::protocol

#include "varNum.hpp"

#endif  // VARNUM_H
