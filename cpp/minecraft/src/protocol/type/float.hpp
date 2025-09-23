// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file float.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/11 21:45
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef FLOAT_HPP
#define FLOAT_HPP
#pragma once

#include "../../utils/utils.h"
#include <bit>

namespace minecraft::protocol {
    inline Float::Float(float value)
        : value_(value) {}

    constexpr std::size_t Float::size() noexcept { return size_; }

    inline Float::type Float::value() const noexcept { return value_; }

    inline Float::encodeType Float::encode() const {
        if (!cached) {
            uint32_t intValue;

            std::memcpy(&intValue, &value_, sizeof(float));

            if constexpr (std::endian::native == std::endian::little) intValue = __builtin_bswap32(intValue);

            for (std::size_t i = 0; i < size_; ++i) data[i] = static_cast<std::byte>(intValue >> (24 - i * 8) & 0xFF);

            cached = true;
        }

        return data;
    }

    inline auto Float::decode(const std::byte* data) {
        uint32_t intValue = 0;

        for (std::size_t i = 0; i < size_; ++i) intValue |= static_cast<uint32_t>(data[i]) << (24 - i * 8);

        if constexpr (std::endian::native == std::endian::little) intValue = __builtin_bswap32(intValue);

        float result;
        std::memcpy(&result, &intValue, sizeof(float));

        return result;
    }

    inline std::string Float::toString() const { return std::to_string(value_); }

    inline std::string Float::toHexString() const { return minecraft::toHexString(encode()); }

}  // namespace minecraft::protocol

#endif  // FLOAT_HPP
