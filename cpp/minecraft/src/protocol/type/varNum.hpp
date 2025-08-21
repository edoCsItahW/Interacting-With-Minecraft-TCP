// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file varNum.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/17 01:53
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef VARNUM_HPP
#define VARNUM_HPP
#pragma once

#include "../../utils/utils.h"

namespace minecraft::protocol::detail {
    template<typename T>
    constexpr std::size_t varNumSize(T value) {
        std::size_t size = 0;

        std::make_unsigned_t<T> uvalue = value;

        do {
            uvalue >>= 7;
            size++;
        } while (uvalue != 0);

        return size;
    }

    template<intOrLong T, typename C>
    constexpr C varNumSerialize(T value, void(func)(C &, std::size_t, std::byte)) {
        C result{};
        std::make_unsigned_t<T> uvalue = value;

        for (std::size_t i = 0; uvalue != 0; i++) {
            auto byte = static_cast<std::byte>(uvalue & SEGMENT_BITS<int>);

            uvalue >>= 7;

            if (uvalue != 0) byte |= CONTINUE_BIT<std::byte>;

            func(result, i, byte);
        }

        return result;
    }

    template<intOrLong T>
    T varNumDeserialize(const std::byte *value) {
        using UT  = std::make_unsigned_t<T>;
        UT result = 0;
        int shift = 0;

        for (std::size_t i = 0;; i++) {
            result |= static_cast<UT>(value[i] & SEGMENT_BITS<std::byte>) << shift;

            if ((value[i] & CONTINUE_BIT<std::byte>) == static_cast<std::byte>(0)) return result;

            shift += 7;

            if (shift >= sizeof(T) * 8) throw std::runtime_error("VarNum overflow");
        }
    }

    template<intOrLong T>
    VarNum<T>::VarNum()
        : value(0)
        , size(0) {}

    template<intOrLong T>
    VarNum<T>::VarNum(T value)
        : value(value)
        , size(varNumSize(value)) {}

    template<intOrLong T>
    auto VarNum<T>::serialize() const {
        using R = std::vector<std::byte>;

        if (data.empty()) data = std::move(varNumSerialize<T, R>(value, [](R &result, std::size_t, std::byte byte) { result.push_back(byte); }));

        return data;
    }

    template<intOrLong T>
    auto VarNum<T>::deserialize(const std::byte *data) {
        return VarNum(varNumDeserialize<T>(data));
    }

    template<intOrLong T>
    std::string VarNum<T>::toString() const {
        return std::to_string(value);
    }

    template<intOrLong T>
    std::string VarNum<T>::toHexString() const {
        return minecraft::toHexString(serialize());
    }

    template<intOrLong T, T V>
    constexpr auto VarNum<T, V>::serialize() {
        using R = std::array<std::byte, varNumSize<T>(V)>;
        return varNumSerialize<T, R>(V, [](R &result, std::size_t i, std::byte byte) { result[i] = byte; });
    }

    template<intOrLong T, T V>
    auto VarNum<T, V>::deserialize(const std::byte *data) {
        return VarNum<T>::deserialize(data);
    }

    template<intOrLong T, T V>
    std::string VarNum<T, V>::toString() {
        return std::to_string(V);
    }

    template<intOrLong T, T V>
    std::string VarNum<T, V>::toHexString() {
        return minecraft::toHexString(serialize());
    }

}  // namespace minecraft::protocol::detail

#endif  // VARNUM_HPP
