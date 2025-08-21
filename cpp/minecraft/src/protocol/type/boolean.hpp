// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file boolean.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 20:38
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef BOOLEAN_HPP
#define BOOLEAN_HPP
#pragma once

#include <array>

namespace minecraft::protocol {
    inline Boolean<>::Boolean()
        : value(false) {}

    inline Boolean<>::Boolean(const bool value)
        : value(value) {}

    inline auto Boolean<>::serialize() const {
        if (data.empty()) data = std::array{value ? std::byte{1} : std::byte{0}};

        return data;
    }

    inline auto Boolean<>::deserialize(const std::byte* data) { return Boolean{*reinterpret_cast<const bool*>(data)}; }

    inline std::string Boolean<>::toString() const { return value ? "true" : "false"; }

    inline std::string Boolean<>::toHexString() const { return value ? "0x01" : "0x00"; }

    template<bool V>
    constexpr auto Boolean<V>::serialize() {
        return std::array{std::byte{V ? 1 : 0}};
    }

    template<bool V>
    auto Boolean<V>::deserialize(const std::byte* data) {
        return Boolean<>::deserialize(data);
    }

    template<bool V>
    std::string Boolean<V>::toString() {
        return V ? "true" : "false";
    }

    template<bool V>
    std::string Boolean<V>::toHexString() {
        return V ? "0x01" : "0x00";
    }

}  // namespace minecraft::protocol

#endif  // BOOLEAN_HPP
