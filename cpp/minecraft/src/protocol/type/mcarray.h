// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file mcarray.h 
 * @author edocsitahw 
 * @version 1.1
 * @date 2025/09/11 18:15
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef MCARRAY_H
#define MCARRAY_H
#pragma once

#include "varNum.h"
#include <string>
#include <vector>

namespace minecraft::protocol {

    template<typename T = std::byte>
    struct Array {
    private:
        std::size_t size_ = 0;

        std::vector<T> value_;

        mutable bool cached = false;

        mutable std::vector<std::byte> data{};

        Array(const std::vector<T>& value);

    public:
        using type = std::vector<T>;

        using serializeType = std::vector<std::byte>;

        Array() = default;

        [[nodiscard]] std::size_t size() const;

        [[nodiscard]] type value() const;

        [[nodiscard]] serializeType serialize() const;

        static auto deserialize(const std::byte* data, const VarInt& sizeField);

        static auto deserialize(const std::byte* data, std::size_t size);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<typename>
    struct isArrayField : std::false_type {};

    template<typename T>
    struct isArrayField<Array<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool isArrayField_v = isArrayField<T>::value;

    template<typename T>
    concept is_array_field = isArrayField_v<T>;
}

#include "mcarray.hpp"

#endif //MCARRAY_H
