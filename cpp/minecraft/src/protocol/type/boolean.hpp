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
    inline Boolean::Boolean(const bool value)
        : value_(value) {}

    constexpr std::size_t Boolean::size() { return size_; }

    inline Boolean::type Boolean::value() const { return value_; }

    inline Boolean::serializeType Boolean::serialize() const {
        if (data.empty()) data = std::array{value_ ? std::byte{1} : std::byte{0}};

        return data;
    }

    inline auto Boolean::deserialize(const std::byte* data) { return Boolean{*reinterpret_cast<const bool*>(data)}; }

    inline std::string Boolean::toString() const { return value_ ? "true" : "false"; }

    inline std::string Boolean::toHexString() const { return value_ ? "0x01" : "0x00"; }

}  // namespace minecraft::protocol

#endif  // BOOLEAN_HPP
