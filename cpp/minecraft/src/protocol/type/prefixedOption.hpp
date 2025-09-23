// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file prefixedOption.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/14 14:51
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef PREFIXEDOPTION_HPP
#define PREFIXEDOPTION_HPP
#pragma once

#include "../../utils/utils.h"
#include "boolean.h"

namespace minecraft::protocol {

    template<typename T>
    PrefixedOption<T>::PrefixedOption()
        : size_(0)
        , value_(std::nullopt) {}

    template<typename T>
    PrefixedOption<T>::PrefixedOption(const std::optional<T>& value)
        : value_(value) {
        size_ = Boolean::size();

        if (value.has_value()) {
            if constexpr (requires { T::size(); })
                size_ += value.value().size();
            else
                size_++;
        }
    }

    template<typename T>
    std::size_t PrefixedOption<T>::size() const {
        return size_;
    }

    template<typename T>
    typename PrefixedOption<T>::type PrefixedOption<T>::value() const {
        return value_;
    }

    template<typename T>
    typename PrefixedOption<T>::encodeType PrefixedOption<T>::encode() const {
        auto boolBytes = Boolean(value_.has_value()).encode();
        encodeType result{boolBytes.begin(), boolBytes.end()};

        if (value_.has_value()) {
            if constexpr (requires { T::encode(); }) {
                auto valueBytes = value_.value().encode();

                result.insert_range(result.end(), valueBytes);
            } else
                result.push_back(static_cast<std::byte>(value_.value()));
        }

        return result;
    }

    template<typename T>
    auto PrefixedOption<T>::decode(const std::byte* data) {
        const auto boolValue = Boolean::decode(data).value();
        data += Boolean::size();

        if (boolValue) {
            if constexpr (requires { T::deencode(data); })
                return PrefixedOption(T::deencode(data));

            else
                return PrefixedOption(static_cast<T>(*data));
        }

        return PrefixedOption();
    }

    template<typename T>
    std::string PrefixedOption<T>::toString() const {
        if (value_.has_value()) {
            if constexpr (requires { T::toString(); })
                return value_.value().toString();
            else
                return std::to_string(value_.value());
        }

        return "null";
    }

    template<typename T>
    std::string PrefixedOption<T>::toHexString() const {
        return minecraft::toHexString(encode());
    }

}  // namespace minecraft::protocol

#endif  // PREFIXEDOPTION_HPP
