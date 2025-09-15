// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file angle.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/11 22:07
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef ANGLE_H
#define ANGLE_H
#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace minecraft::protocol {
    struct Angle {
    private:
        static constexpr std::size_t size_ = 1;

        static constexpr float DEGREES_PER_STEP = 360.0f / 256.0f;
        static constexpr float STEPS_PER_DEGREE = 256.0f / 360.0f;

        mutable std::array<uint8_t, size_> data{};

        uint8_t value_ = 0;

        template<typename T>
        uint8_t normalize(T degrees);

        mutable bool cached = false;

    public:
        using type = uint8_t;

        using serializeType = std::array<uint8_t, size_>;

        Angle() = default;

        template<typename T>
        Angle(T value);

        [[nodiscard]] static constexpr std::size_t size();

        [[nodiscard]] type value() const;

        [[nodiscard]] serializeType serialize() const;

        static auto deserialize(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;

        float toDegrees() const;

        float toRadians() const;
    };

    template<typename T>
    concept is_angle_field = std::is_same_v<T, Angle>;

}  // namespace minecraft::protocol

#include "angle.hpp"

#endif  // ANGLE_H
