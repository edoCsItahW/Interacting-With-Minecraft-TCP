// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file varNum.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/17 01:53
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef VARNUM_HPP
#define VARNUM_HPP
#pragma once

#include "../../utils/utils.h"

namespace minecraft::protocol::detail {
    template<typename T>
    constexpr std::size_t varNumSize(T value) {
        std::size_t size = 0;

        std::make_unsigned_t<T> uvalue = value;

        do {
            uvalue >>= 7;
            size++;
        } while (uvalue != 0);

        return size;
    }

    template<intOrLong T>
    VarNum<T>::VarNum()
        : size_(0) {}

    template<intOrLong T>
    VarNum<T>::VarNum(T value)
        : size_(varNumSize(value))
        , value_(value) {}

    template<intOrLong T>
    std::size_t VarNum<T>::size() const {
        return size_;
    }

    template<intOrLong T>
    typename VarNum<T>::type VarNum<T>::value() const {
        return value_;
    }

    template<intOrLong T>
    typename VarNum<T>::encodeType VarNum<T>::encode() const {
        if (data.empty()) {
            std::make_unsigned_t<T> uvalue = value_;

            do {
                auto byte = static_cast<std::byte>(uvalue & SEGMENT_BITS<int>);

                uvalue >>= 7;

                if (uvalue != 0) byte |= CONTINUE_BIT<std::byte>;

                data.push_back(byte);
            } while (uvalue != 0);
        }

        return data;
    }

    template<intOrLong T>
    auto VarNum<T>::decode(const std::byte *data) {
        using UT  = std::make_unsigned_t<T>;
        UT result = 0;
        int shift = 0;

        for (std::size_t i = 0;; i++) {
            result |= static_cast<UT>(data[i] & SEGMENT_BITS<std::byte>) << shift;

            if ((data[i] & CONTINUE_BIT<std::byte>) == static_cast<std::byte>(0)) return VarNum(result);

            shift += 7;

            if (shift >= sizeof(T) * 8) throw std::runtime_error("VarNum overflow");
        }
    }

    template<intOrLong T>
    std::string VarNum<T>::toString() const {
        return std::to_string(value_);
    }

    template<intOrLong T>
    std::string VarNum<T>::toHexString() const {
        return minecraft::toHexString(encode());
    }

}  // namespace minecraft::protocol::detail

#endif  // VARNUM_HPP
