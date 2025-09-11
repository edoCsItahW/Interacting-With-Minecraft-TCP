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

    struct String {
    private:
        mutable std::vector<std::byte> data;

        std::size_t size_;

        std::string value_;

    public:
        using type = std::string;

        using serializeType = std::vector<std::byte>;

        String();

        String(const std::string &value);

        String(const String &other) = default;

        String(String &&other) = default;

        String &operator=(const String &other) = default;

        String &operator=(String &&other) = default;

        [[nodiscard]] std::size_t size() const;

        [[nodiscard]] type value() const;

        [[nodiscard]] serializeType serialize() const;

        static auto deserialize(const std::byte *data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<typename T>
    concept is_string_field = std::is_same_v<T, String>;

}  // namespace minecraft::protocol

#include "str.hpp"

#endif  // STR_H
