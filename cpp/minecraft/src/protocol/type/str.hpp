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
    namespace detail {
        template<intOrLong T>
        static std::pair<T, int> parseVarInt(const std::byte *data) {
            T value       = 0;
            int shift     = 0;
            int bytesRead = 0;

            while (true) {
                if (bytesRead >= 5) throw std::runtime_error("VarInt is too long");

                std::byte b = *data++;

                value |= static_cast<T>(b & SEGMENT_BITS<std::byte>) << shift;
                bytesRead++;

                if ((b & CONTINUE_BIT<std::byte>) == std::byte{0}) break;

                shift += 7;

                if (shift >= sizeof(T) * 8) throw std::runtime_error("VarInt is too long");
            }

            return {value, bytesRead};
        }
    }  // namespace detail

    inline String<>::String()
        : size(0) {}

    inline String<>::String(const std::string &value)
        : value(value)
        , size(value.size() + detail::varNumSize(value.size())) {}

    inline auto String<>::serialize() const {
        if (data.empty()) {
            auto varInt    = detail::VarNum(static_cast<int>(value.size()));
            auto sizeBytes = varInt.serialize();

            data.reserve(sizeBytes.size() + value.size());

            data.insert(data.end(), sizeBytes.begin(), sizeBytes.end());

            for (char c : value) data.push_back(static_cast<std::byte>(c));
        }

        return data;
    }

    inline auto String<>::deserialize(const std::byte *data) {
        auto [length, bytesRead] = detail::parseVarInt<int>(data);

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

    inline std::string String<>::toString() const {
        return value;
    }

    inline std::string String<>::toHexString() const { return minecraft::toHexString(value); }

    template<FStrByte V>
    constexpr auto String<V>::serialize() {
        constexpr std::size_t N = V.data.size();

        constexpr auto varInt    = VarInt<static_cast<int>(N)>();
        constexpr auto sizeBytes = varInt.serialize();

        constexpr std::size_t size = sizeBytes.size() + N;
        std::array<std::byte, size> result{};

        std::copy(sizeBytes.begin(), sizeBytes.end(), result.begin());

        std::copy(V.data.begin(), V.data.end(), result.begin() + sizeBytes.size());

        return result;
    }

    template<FStrByte V>
    auto String<V>::deserialize(const std::byte *data) {
        return String<>::deserialize(data);
    }

    template<FStrByte V>
    std::string String<V>::toString() {
        std::string result;
        result.reserve(V.data.size());

        for (std::byte b : V.data) result.push_back(static_cast<char>(b));

        return result;
    }

    template<FStrByte V>
    std::string String<V>::toHexString() {
        return minecraft::toHexString(toString());
    }


}  // namespace minecraft::protocol

#endif  // STR_HPP
