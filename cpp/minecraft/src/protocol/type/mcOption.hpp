// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file mcOption.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/14 16:33
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef MCOPTION_HPP
#define MCOPTION_HPP
#pragma once
#include "../../utils/utils.h"

namespace minecraft::protocol {

    template<typename T>
    Option<T>::Option()
        : size_(0)
        , value_(std::nullopt) {}

    template<typename T>
    Option<T>::Option(const std::optional<T>& value)
        : value_(value) {
        if ((size_ = value.has_value())) {
            if constexpr (requires { T::size(); })
                size_ = value.value().size();

            else
                size_ = 1;
        }
    }

    template<typename T>
    std::size_t Option<T>::size() const {
        return size_;
    }

    template<typename T>
    typename Option<T>::type Option<T>::value() const {
        return value_;
    }

    template<typename T>
    typename Option<T>::encodeType Option<T>::encode() const {
        if (!cached) {
            if (value_.has_value()) {
                if constexpr (requires { T::encode(); })
                    data = value_.value().encode();

                else
                    data.push_back(static_cast<std::byte>(*value_));
            }

            cached = true;
        }

        return data;
    }

    template<typename T>
    auto Option<T>::decode(const std::byte* data, const Boolean& boolField) {
        if (boolField.value()) {
            if constexpr (requires { T::decode(data); })
                return Option{T::decode(data)};

            else
                return Option{static_cast<T>(*data)};
        }

        return Option{};
    }

    template<typename T>
    std::string Option<T>::toString() const {
        if (value_.has_value()) {
            if constexpr (requires (T t) { t.toString(); })
                return value_.value().toString();

            else if constexpr (requires { std::to_string(*value_); })
                return std::to_string(value_.value());

            else
                return std::to_string(static_cast<std::byte>(*value_));
        }

        return "null";
    }

    template<typename T>
    std::string Option<T>::toHexString() const {
        if (value_.has_value()) return minecraft::toHexString(encode());

        return "";
    }

}  // namespace minecraft::protocol

#endif  // MCOPTION_HPP
