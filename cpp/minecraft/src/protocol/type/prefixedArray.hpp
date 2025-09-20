// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file prefixedArray.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/13 23:44
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef PREFIXEDARRAY_HPP
#define PREFIXEDARRAY_HPP
#pragma once

#include "prefixedArray.h"
#include "str.h"
#include <sstream>

namespace minecraft::protocol {

    template<typename T>
    PrefixedArray<T>::PrefixedArray()
        : value_()
        , size_(0) {}

    template<typename T>
    PrefixedArray<T>::PrefixedArray(type value)
        : value_(value) {
        size_ = detail::varNumSize(value.size());

        for (auto& elem : value_) size_ += elem.size();
    }

    template<typename T>
    std::size_t PrefixedArray<T>::size() const {
        return size_;
    }

    template<typename T>
    typename PrefixedArray<T>::type PrefixedArray<T>::value() const {
        return value_;
    }

    template<typename T>
    typename PrefixedArray<T>::serializeType PrefixedArray<T>::serialize() const {
        if (!cached) {
            data = VarInt(size_).serialize();

            for (const auto& elem : value_) data.insert_range(data.end(), elem.serialize());

            cached = true;
        }

        return data;
    }

    template<typename T>
    auto PrefixedArray<T>::deserialize(const std::byte* data) {
        auto [count, countShift] = parseVarInt<int>(data);
        data += countShift;

        std::vector<T> resVec;

        for (int i = 0; i < count; i++) {
            auto elem = T::deserialize(data);

            resVec.push_back(elem);

            data += elem.size();
        }

        return PrefixedArray(resVec);
    }

    template<typename T>
    std::string PrefixedArray<T>::toString() const {
        std::ostringstream oss;

        oss << "[";

        for (std::size_t i{0}; i < value_.size(); i++) {
            if constexpr (requires { value_[i].toString(); })
                oss << value_[i].toString();

            else
                oss << value_[i];

            oss << (i == value_.size() - 1 ? "" : ", ");
        }

        oss << "]";

        return oss.str();
    }

    template<typename T>
    std::string PrefixedArray<T>::toHexString() const {
        return minecraft::toHexString(serialize());
    }

}  // namespace minecraft::protocol

#endif  // PREFIXEDARRAY_HPP
