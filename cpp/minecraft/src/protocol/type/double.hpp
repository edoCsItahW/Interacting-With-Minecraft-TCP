// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file double.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/11 21:58
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef DOUBLE_HPP
#define DOUBLE_HPP
#pragma once

#include "../../utils/utils.h"
#include <bit>

namespace minecraft::protocol {
    inline Double::Double(double value)
        : value_(value) {}

    constexpr std::size_t Double::size() noexcept { return size_; }

    inline Double::type Double::value() const noexcept { return value_; }

    inline Double::serializeType Double::serialize() const {
        if (!cached) {
            uint64_t intValue;

            std::memcpy(&intValue, &value_, sizeof(double));

            if constexpr (std::endian::native == std::endian::little) intValue = __builtin_bswap64(intValue);

            for (std::size_t i = 0; i < size_; ++i) data[i] = static_cast<std::byte>(intValue >> (56 - i * 8) & 0xFF);

            cached = true;
        }

        return data;
    }

    inline auto Double::deserialize(const std::byte* data) {
        uint64_t intValue = 0;

        for (std::size_t i = 0; i < size_; ++i) intValue |= static_cast<uint64_t>(data[i]) << (56 - i * 8);

        if constexpr (std::endian::native == std::endian::little) intValue = __builtin_bswap64(intValue);

        double value;
        std::memcpy(&value, &intValue, sizeof(double));

        return value;
    }

    inline std::string Double::toString() const { return std::to_string(value_); }

    inline std::string Double::toHexString() const { return minecraft::toHexString(serialize()); }


}  // namespace minecraft::protocol

#endif  // DOUBLE_HPP
