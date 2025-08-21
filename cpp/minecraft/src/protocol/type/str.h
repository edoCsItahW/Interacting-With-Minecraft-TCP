// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file str.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/17 15:00
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef STR_H
#define STR_H
#pragma once

#include "../../utils/fstr.h"
#include <string>
#include <vector>

namespace minecraft::protocol {

    template<FStrByte...>
    struct String;

    template<>
    struct String<> {
    private:
        mutable std::vector<std::byte> data;

    public:
        using type = std::string;

        std::string value;

        std::size_t size;

        String();

        String(const std::string &value);

        [[nodiscard]] auto serialize() const;

        static auto deserialize(const std::byte *data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    String(std::string_view) -> String<>;

    template<FStrByte V>
    struct String<V> {
        using type = std::array<std::byte, V.size>;

        static constexpr auto value = V.data;

        static constexpr auto size = V.size;

        static constexpr auto serialize();

        static auto deserialize(const std::byte *data);

        static std::string toString();

        static std::string toHexString();
    };

    using StringType = String<>;

    template<typename>
    struct is_string_field : std::false_type {};

    template<>
    struct is_string_field<String<>> : std::true_type {};

    template<FStrByte V>
    struct is_string_field<String<V>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_string_field_v = is_string_field<T>::value;

    template<typename T>
    concept string_field = is_string_field_v<T>;

}  // namespace minecraft::protocol

#include "str.hpp"

#endif  // STR_H
