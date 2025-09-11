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

    struct Boolean {
    private:
        mutable std::array<std::byte, 1> data{};

        static constexpr std::size_t size_ = 1;

        bool value_;

    public:
        using type = bool;

        using serializeType = std::array<std::byte, 1>;

        Boolean();

        Boolean(bool value);

        Boolean(const Boolean& other) = default;

        Boolean(Boolean&& other) = default;

        Boolean& operator=(const Boolean& other) = default;

        Boolean& operator=(Boolean&& other) = default;

        [[nodiscard]] static constexpr std::size_t size();

        [[nodiscard]] type value() const;

        [[nodiscard]] serializeType serialize() const;

        static auto deserialize(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<typename T>
    concept is_boolean_field = std::is_same_v<T, Boolean>;

}  // namespace minecraft::protocol

#include "boolean.hpp"

#endif  // BOOLEAN_H
