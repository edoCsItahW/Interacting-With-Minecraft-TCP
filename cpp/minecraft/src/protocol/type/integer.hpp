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

    template<typename T>
    Integer<T>::Integer()
        : value_(0) {}

    template<typename T>
    Integer<T>::Integer(T value)
        : value_(value) {}

    template<typename T>
    constexpr std::size_t Integer<T>::size() {
        return size_;
    }

    template<typename T>
    typename Integer<T>::type Integer<T>::value() const {
        return value_;
    }

    template<typename T>
    typename Integer<T>::encodeType Integer<T>::encode() const {
        if (!cached) {
            using UT  = std::make_unsigned_t<T>;
            UT uvalue = static_cast<UT>(value_);

            if constexpr (std::endian::native == std::endian::big)  // 大端序
                std::memcpy(data.data(), &value_, size_);

            else if constexpr (std::endian::native == std::endian::little) {
                // 小端序
                for (std::size_t i = 0; i < size_; ++i) data[i] = static_cast<std::byte>(uvalue >> (CHAR_BIT * (size_ - 1 - i))) & std::byte{0xFF};
            } else
                throw std::runtime_error("Unsupported endianness");

            cached = true;
        }

        return data;
    }

    template<typename T>
    auto Integer<T>::decode(const std::byte *data) {
        constexpr auto size = sizeof(T);

        using UT  = std::make_unsigned_t<T>;
        UT result = 0;

        if constexpr (std::endian::native == std::endian::big)  // 大端序
            std::memcpy(&result, data, size);

        else if constexpr (std::endian::native == std::endian::little)
            for (std::size_t i = 0; i < size; ++i) result |= static_cast<UT>(static_cast<unsigned char>(data[i])) << (CHAR_BIT * (size - 1 - i));
        else
            throw std::runtime_error("Unsupported endianness");

        return Integer(result);
    }

    template<typename T>
    std::string Integer<T>::toString() const {
        return std::to_string(value_);
    }

    template<typename T>
    std::string Integer<T>::toHexString() const {
        return std::bitset<size_ * 8>(value_).to_string();
    }

}  // namespace minecraft::protocol::detail


#endif  // INTEGER_HPP
