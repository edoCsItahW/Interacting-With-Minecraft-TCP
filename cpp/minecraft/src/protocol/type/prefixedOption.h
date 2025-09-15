// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file prefixedOption.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/14 14:46
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef PREFIXEDOPTION_H
#define PREFIXEDOPTION_H
#pragma once

#include <optional>
#include <string>
#include <vector>

namespace minecraft::protocol {
    template<typename T>
    struct PrefixedOption {
    private:
        mutable std::vector<std::byte> data{};

        mutable bool cached = false;

        std::size_t size_ = 0;

        std::optional<T> value_;

    public:
        using type = std::optional<T>;

        using serializeType = std::vector<std::byte>;

        PrefixedOption() = default;

        PrefixedOption(const std::optional<T>& value = std::nullopt);

        [[nodiscard]] std::size_t size() const;

        [[nodiscard]] type value() const;

        [[nodiscard]] serializeType serialize() const;

        static auto deserialize(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<typename>
    struct isPrefixedOptionField : std::false_type {};

    template<typename T>
    struct isPrefixedOptionField<PrefixedOption<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool isPrefixedOption_v = isPrefixedOptionField<T>::value;

    template<typename T>
    concept is_prefixed_option_field = isPrefixedOption_v<T>;

}  // namespace minecraft::protocol

#include "prefixedOption.hpp"

#endif  // PREFIXEDOPTION_H
