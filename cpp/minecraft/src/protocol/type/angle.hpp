// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file angle.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/11 22:12
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef ANGLE_HPP
#define ANGLE_HPP
#pragma once

#include "../../utils/utils.h"
#include <cmath>
#include <numbers>
#include <sstream>

namespace minecraft::protocol {

    template<typename T>
    uint8_t Angle::normalize(T degrees) {
        degrees = std::fmod(degrees, 360.0);

        if (degrees < 0) degrees += 360.0;

        return static_cast<uint8_t>(static_cast<int>(std::round(degrees * STEPS_PER_DEGREE)) % 256);
    }

    template<typename T>
    Angle::Angle(T value)
        : value_(std::is_same_v<T, uint8_t> ? value : normalize(value)) {}

    constexpr std::size_t Angle::size() { return size_; }

    inline Angle::type Angle::value() const { return value_; }

    inline Angle::serializeType Angle::serialize() const {
        if (!cached) {
            data[0] = value_;
            cached  = true;
        }

        return data;
    }

    inline auto Angle::deserialize(const std::byte* data) { return Angle(static_cast<uint8_t>(data[0])); }

    inline std::string Angle::toString() const {
        std::ostringstream ss;

        ss << toDegrees() << "° (" << static_cast<int>(value_) << " steps)";

        return ss.str();
    }

    inline std::string Angle::toHexString() const { return minecraft::toHexString(serialize()); }

    inline float Angle::toDegrees() const { return value_ * DEGREES_PER_STEP; }

    inline float Angle::toRadians() const { return toDegrees() * std::numbers::pi_v<float> / 180.0f; }

}  // namespace minecraft::protocol

#endif  // ANGLE_HPP
