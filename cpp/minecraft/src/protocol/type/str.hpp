// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file str.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/17 17:02
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef STR_HPP
#define STR_HPP
#pragma once

#include "../../utils/utils.h"
#include "varNum.h"
#include <stdexcept>

namespace minecraft::protocol {
    template<detail::intOrLong T>
    static std::pair<T, int> parseVarInt(const std::byte *data) {
        T value       = 0;
        int shift     = 0;
        int bytesRead = 0;

        while (true) {
            if (bytesRead >= 5) throw std::runtime_error("VarInt is too long");

            std::byte b = *data++;

            value |= static_cast<T>(b & detail::SEGMENT_BITS<std::byte>) << shift;
            bytesRead++;

            if ((b & detail::CONTINUE_BIT<std::byte>) == std::byte{0}) break;
            shift += 7;

            if (shift >= sizeof(T) * 8) throw std::runtime_error("VarInt is too long");
        }

        return {value, bytesRead};
    }

    inline String::String(const std::string &value)
        : size_(value.size() + detail::varNumSize(value.size()))
        , value_(value) {}

    inline std::size_t String::size() const { return size_; }

    inline String::type String::value() const { return value_; }

    inline String::serializeType String::serialize() const {
        if (!cached) {
            auto varInt    = VarInt(static_cast<int>(value_.size()));
            auto sizeBytes = varInt.serialize();

            data.reserve(sizeBytes.size() + value_.size());

            data.insert(data.end(), sizeBytes.begin(), sizeBytes.end());

            for (char c : value_) data.push_back(static_cast<std::byte>(c));

            cached = true;
        }

        return data;
    }

    inline auto String::deserialize(const std::byte *data) {
        auto [length, bytesRead] = parseVarInt<int>(data);

        std::string result;
        result.reserve(length);

        const std::byte *ptr = data + bytesRead;
        for (int i = 0; i < length; ++i)
            // if (auto c = static_cast<char>(ptr[i]); c == '\0')  // 保护设置
            //     break;
            // else
            result.push_back(static_cast<char>(ptr[i]));

        return String(result);
    }

    inline std::string String::toString() const { return value_; }

    inline std::string String::toHexString() const { return minecraft::toHexString(value_); }

}  // namespace minecraft::protocol

#endif  // STR_HPP
