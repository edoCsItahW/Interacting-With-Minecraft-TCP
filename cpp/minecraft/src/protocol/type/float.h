// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file float.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/11 21:40
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef FLOAT_H
#define FLOAT_H
#pragma once

#include <array>
#include <string>

namespace minecraft::protocol {

    struct Float {
    private:
        mutable std::array<std::byte, 4> data{};

        mutable bool cached = false;


        static constexpr std::size_t size_ = 4;

        float value_ = 0.0f;

    public:
        using type = float;

        using serializeType = std::array<std::byte, size_>;

        [[nodiscard]] static constexpr std::size_t size() noexcept;

        [[nodiscard]] type value() const noexcept;

        Float() = default;

        Float(float value);

        [[nodiscard]] serializeType serialize() const;

        static auto deserialize(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<typename T>
    concept is_float_field = std::is_same_v<T, Float>;

}  // namespace minecraft::protocol

#include "float.hpp"

#endif  // FLOAT_H
