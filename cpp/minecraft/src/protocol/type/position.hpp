// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file position.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/13 17:12
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef POSITION_HPP
#define POSITION_HPP
#pragma once

#include "../../utils/utils.h"
#include "integer.h"

namespace minecraft::protocol {

    inline Position::Position()
        : x_(0)
        , y_(0)
        , z_(0) {}

    inline Position::Position(const int64_t x, const int64_t y, const int64_t z)
        : x_(x)
        , y_(y)
        , z_(z) {}

    inline Position::Position(const type& value)
        : value_(value)
        , x_(0)
        , y_(0)
        , z_(0) {}

    inline std::size_t Position::size() { return size_; }

    inline Position::type Position::value() const { return value_; }

    inline Position::serializeType Position::serialize() const {
        if (!cached) {
            const auto v = (x_ & 0x3FFFFFF) << 38 | (y_ & 0xFFF) << 26 | z_ & 0x3FFFFFF;

            for (auto i = 0; i < size_; i++) data[i] = static_cast<std::byte>(v >> (56 - i * 8) & 0xFF);

            cached = true;
        }

        return data;
    }

    inline auto Position::deserialize(const std::byte* data) {
        const auto v = Long::deserialize(data).value();

        auto x = v >> 38;
        auto y = (v << 52) >> 52;
        auto z = (v << 26) >> 38;

        if (x >= 1 << 25) x -= 1 << 26;
        if (y >= 1 << 11) y -= 1 << 12;
        if (z >= 1 << 25) z -= 1 << 26;

        return Position{x, y, z};
    }

    inline std::string Position::toString() const { return "(" + std::to_string(x_) + ", " + std::to_string(y_) + ", " + std::to_string(z_) + ")"; }

    inline std::string Position::toHexString() const { return minecraft::toHexString(serialize()); }


}  // namespace minecraft::protocol

#endif  // POSITION_HPP
