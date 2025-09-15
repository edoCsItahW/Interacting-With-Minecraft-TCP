// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file mcOption.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/14 11:40
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef MCOPTION_H
#define MCOPTION_H
#pragma once

#include <optional>
#include <string>
#include <vector>

namespace minecraft::protocol {

    template<typename T>
    struct Option {
    private:
        mutable std::vector<std::byte> data{};

        mutable bool cached = false;

        std::size_t size_ = 0;

        std::optional<T> value_;

    public:
        using type = std::optional<T>;

        using serializeType = std::vector<std::byte>;

        Option() = default;

        Option(const std::optional<T>& value = std::nullopt);

        [[nodiscard]] std::size_t size() const;

        [[nodiscard]] type value() const;

        [[nodiscard]] serializeType serialize() const;

        static auto deserialize(const std::byte* data, const Boolean& boolField);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<typename>
    struct isOptionField : std::false_type {};

    template<typename T>
    struct isOptionField<Option<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool isOptionField_v = isOptionField<T>::value;

    template<typename T>
    concept is_option_field = isOptionField_v<T>;

}  // namespace minecraft::protocol

#include "mcOption.hpp"

#endif  // MCOPTION_H
