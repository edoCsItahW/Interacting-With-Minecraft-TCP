// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file integer.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/17 12:18
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef INTEGER_HPP
#define INTEGER_HPP
#pragma once

#include <bit>
#include <bitset>
#include <stdexcept>

namespace minecraft::protocol::detail {
    template<typename T, std::size_t N>
    constexpr std::array<std::byte, N> intSerialize(T value) {
        std::array<std::byte, N> result;

        using UT  = std::make_unsigned_t<T>;
        UT uvalue = static_cast<UT>(value);

        if constexpr (std::endian::native == std::endian::big)  // 大端序
            std::memcpy(result.data(), &value, N);

        else if constexpr (std::endian::native == std::endian::little) {
            // 小端序
            for (std::size_t i = 0; i < N; ++i) result[i] = static_cast<std::byte>(uvalue >> (CHAR_BIT * (N - 1 - i))) & std::byte{0xFF};
        } else
            throw std::runtime_error("Unsupported endianness");

        return result;
    }

    template<typename T>
    T intDeserialize(const std::byte *data) {
        constexpr auto size = sizeof(T);

        using UT  = std::make_unsigned_t<T>;
        UT result = 0;

        if constexpr (std::endian::native == std::endian::big)  // 大端序
            std::memcpy(&result, data, size);

        else if constexpr (std::endian::native == std::endian::little)
            for (std::size_t i = 0; i < size; ++i) result |= static_cast<UT>(static_cast<unsigned char>(data[i])) << (CHAR_BIT * (size - 1 - i));
        else
            throw std::runtime_error("Unsupported endianness");

        return result;
    }

    template<typename T>
    Integer<T>::Integer()
        : value(0) {}

    template<typename T>
    Integer<T>::Integer(T value)
        : value(value) {}

    template<typename T>
    auto Integer<T>::serialize() const {
        if (!cached) {
            data   = intSerialize<T, size>(value);
            cached = true;
        }

        return data;
    }

    template<typename T>
    auto Integer<T>::deserialize(const std::byte *data) {
        return Integer(intDeserialize<T>(data));
    }

    template<typename T>
    std::string Integer<T>::toString() const {
        return std::to_string(value);
    }

    template<typename T>
    std::string Integer<T>::toHexString() const {
        return std::bitset<size * 8>(value).to_string();
    }

    template<typename T, T V>
    constexpr auto Integer<T, V>::serialize() const {
        return intSerialize<T, size>(V);
    }

    template<typename T, T V>
    auto Integer<T, V>::deserialize(const std::byte *data) {
        return Integer<T>::deserialize(data);
    }

    template<typename T, T V>
    std::string Integer<T, V>::toString() {
        return std::to_string(V);
    }

    template<typename T, T V>
    std::string Integer<T, V>::toHexString() {
        return std::bitset<size * 8>(V).to_string();
    }
}  // namespace minecraft::protocol::detail


#endif  // INTEGER_HPP
