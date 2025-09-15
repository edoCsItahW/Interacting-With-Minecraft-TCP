// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file double.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/11 21:55
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef DOUBLE_H
#define DOUBLE_H
#pragma once

#include <array>
#include <string>

namespace minecraft::protocol {
    struct Double {
    private:
        mutable std::array<std::byte, 8> data{};

        mutable bool cached = false;

        static constexpr std::size_t size_ = 8;

        double value_ = 0.0;

    public:
        using type = double;

        using serializeType = std::array<std::byte, size_>;

        [[nodiscard]] static constexpr std::size_t size() noexcept;

        [[nodiscard]] type value() const noexcept;

        Double() = default;

        Double(double value);

        [[nodiscard]] serializeType serialize() const;

        static auto deserialize(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<typename T>
    concept is_double_field = std::is_same_v<T, Double>;

}  // namespace minecraft::protocol

#include "double.hpp"

#endif  // DOUBLE_H
