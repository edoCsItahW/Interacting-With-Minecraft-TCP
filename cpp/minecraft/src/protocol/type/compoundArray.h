// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file compoundArray.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/16 21:33
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef COMPOUNDARRAY_H
#define COMPOUNDARRAY_H
#pragma once

#include <string>
#include <tuple>
#include <vector>

namespace minecraft::protocol {

    template<typename... Ts>
    struct CompoundArray {
    private:
        std::size_t size_ = 0;

        std::tuple<Ts...> value_;

        mutable bool cached = false;

        mutable std::vector<std::byte> data{};

        CompoundArray(std::tuple<Ts...>&& value);

    public:
        using type = std::tuple<Ts...>;

        using serializeType = std::vector<std::byte>;

        CompoundArray() = default;

        CompoundArray(Ts&&... args);

        [[nodiscard]] std::size_t size() const;

        [[nodiscard]] type value() const;

        [[nodiscard]] serializeType serialize() const;

        static auto deserialize(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<typename>
    struct isCompoundArrayField : std::false_type {};

    template<typename... Ts>
    struct isCompoundArrayField<CompoundArray<Ts...>> : std::true_type {};

    template<typename T>
    inline constexpr bool isCompoundArray_v = isCompoundArrayField<T>::value;

    template<typename T>
    concept is_compound_array_field = isCompoundArray_v<T>;

}  // namespace minecraft::protocol

#include "compoundArray.hpp"

#endif  // COMPOUNDARRAY_H
