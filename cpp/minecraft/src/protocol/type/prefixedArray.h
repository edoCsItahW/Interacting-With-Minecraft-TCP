// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file prefixedArray.h 
 * @author edocsitahw 
 * @version 1.1
 * @date 2025/09/13 23:33
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef PREFIXEDARRAY_H
#define PREFIXEDARRAY_H
#pragma once

#include <vector>
#include <string>

namespace minecraft::protocol {

    template<typename T>
    struct PrefixedArray {
    private:
        std::vector<T> value_;

        mutable std::vector<std::byte> data{};

        mutable bool cached = false;

        std::size_t size_;

    public:
        using type = std::vector<T>;

        using serializeType = std::vector<std::byte>;

        PrefixedArray() = default;

        PrefixedArray(type value);

        [[nodiscard]] std::size_t size() const;

        [[nodiscard]] type value() const;

        [[nodiscard]] serializeType serialize() const;

        static auto deserialize(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<typename>
    struct isPrefixedArrayField : std::false_type {};

    template<typename T>
    struct isPrefixedArrayField<PrefixedArray<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool isPrefixedArrayField_v = isPrefixedArrayField<T>::value;

    template<typename T>
    concept is_prefixed_array_field = isPrefixedArrayField_v<T>;

}

#include "prefixedArray.hpp"

#endif //PREFIXEDARRAY_H
