// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file mcarray.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/11 18:39
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef MCARRAY_HPP
#define MCARRAY_HPP
#pragma once

#include "../../utils/utils.h"
#include <sstream>

namespace minecraft::protocol {

    template<typename T>
    Array<T>::Array(const std::vector<T>& value)
        : size_(value.size())
        , value_(value) {}

    template<typename T>
    std::size_t Array<T>::size() const {
        return size_;
    }

    template<typename T>
    typename Array<T>::type Array<T>::value() const {
        return value_;
    }

    template<typename T>
    typename Array<T>::encodeType Array<T>::encode() const {
        if (!cached) {
            for (const auto& elem : value_)
                if constexpr (requires { elem.serialize(); })
                    data.insert_range(data.end(), elem.serialize());
                else
                    data.push_back(static_cast<std::byte>(elem));

            cached = true;
        }

        return data;
    }

    template<typename T>
    auto Array<T>::decode(const std::byte* data, const VarInt& sizeField) {
        return decode(data, sizeField.value());
    }

    template<typename T>
    auto Array<T>::decode(const std::byte* data, std::size_t size) {
        type result;

        if constexpr (requires { T::decode(data); })
            while (size > 0) {
                auto elem = T::decode(data);

                result.push_back(elem);

                data += elem.size();
                size -= elem.size();
            }

        else
            for (std::size_t i{0}; i < size; i++)
                result.push_back(static_cast<T>(data[i]));

        return Array(result);
    }

    template<typename T>
    std::string Array<T>::toString() const {
        std::stringstream ss;

        ss << "[";

        for (std::size_t i{0}; const auto& elem : value_) {
            if (i > 0) ss << ", ";

            if constexpr (requires { elem.toString(); })
                ss << elem.toString();
            else if constexpr (requires { std::to_string(elem); })
                ss << std::to_string(elem);
            else
                ss << static_cast<char>(elem);

            i++;
        }

        ss << "]";

        return ss.str();
    }

    template<typename T>
    std::string Array<T>::toHexString() const {
        return minecraft::toHexString(encode());
    }


}  // namespace minecraft::protocol

#endif  // MCARRAY_HPP
