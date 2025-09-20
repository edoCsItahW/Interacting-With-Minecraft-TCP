// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file compoundArray.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/16 21:39
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef COMPOUNDARRAY_HPP
#define COMPOUNDARRAY_HPP
#pragma once

#include "../../utils/utils.h"
#include <sstream>

namespace minecraft::protocol {

    template<typename... Ts>
    CompoundArray<Ts...>::CompoundArray(Ts&&... args)
        : value_{std::forward<Ts>(args)...} {}

    template<typename... Ts>
    CompoundArray<Ts...>::CompoundArray(std::tuple<Ts...>&& value)
        : value_(std::move(value)){};

    template<typename... Ts>
    std::size_t CompoundArray<Ts...>::size() const {
        return size_;
    }

    template<typename... Ts>
    typename CompoundArray<Ts...>::type CompoundArray<Ts...>::value() const {
        return value_;
    }

    template<typename... Ts>
    typename CompoundArray<Ts...>::serializeType CompoundArray<Ts...>::serialize() const {
        if (!cached) {
            [this]<std::size_t... Is>(std::index_sequence<Is...>) {
                if constexpr (requires { std::get<Is>(value_).serialize(); })
                    data.insert_range(data.end(), std::get<Is>(value_).serialize());
                else
                    data.push_back(static_cast<std::byte>(std::get<Is>(value_)));
            }(std::make_index_sequence<sizeof...(Ts)>{});

            cached = true;
        }

        return data;
    }

    template<typename... Ts>
    auto CompoundArray<Ts...>::deserialize(const std::byte* data) {
        type result;

        [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            std::size_t shift = 0;

            (..., [&] {
                if constexpr (requires { std::get<Is>(result).deserialize(data); }) {
                    std::get<Is>(result) = std::get<Is>(value_).deserialize(data + shift);

                    shift += std::get<Is>(result).size();
                }

                else
                    std::get<Is>(result) = static_cast<std::tuple_element_t<Is, type>>(*data++);
            }());
        }(std::make_index_sequence<sizeof...(Ts)>{});

        return CompoundArray(std::move(result));
    }

    template<typename... Ts>
    std::string CompoundArray<Ts...>::toString() const {
        std::stringstream ss;

        ss << "[";

        [&, this]<std::size_t... Is>(std::index_sequence<Is...>) {
            (..., [&] {
                if (Is != 0) ss << ", ";

                if constexpr (requires { std::get<Is>(value_).toString(); })
                    ss << std::get<Is>(value_).toString();

                else
                    ss << std::get<Is>(value_);
            }());
        }(std::make_index_sequence<sizeof...(Ts)>{});

        ss << "]";

        return ss.str();
    }

    template<typename... Ts>
    std::string CompoundArray<Ts...>::toHexString() const {
        return minecraft::toHexString(serialize());
    }


}  // namespace minecraft::protocol

#endif  // COMPOUNDARRAY_HPP
