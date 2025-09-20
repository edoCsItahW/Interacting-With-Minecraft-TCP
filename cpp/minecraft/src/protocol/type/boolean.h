// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file boolean.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 20:35
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef BOOLEAN_H
#define BOOLEAN_H
#pragma once

#include <array>
#include <cstddef>
#include <string>

namespace minecraft::protocol {

    /** @struct Boolean
     *
     * @if zh
     *
     * @brief 表示布尔字段的结构体。
     * @details 表示Minecraft协议中的布尔数据类型，使用一个字节表示true或false。
     *
     * @else
     *
     * @endif
     * */
    struct Boolean {
    private:
        /**
         * @if zh
         *
         * @brief 该字段的字节数。
         * @details 该字段硬被编码后所占用的字节数。由于Boolean字段使用一个字节表示，因此该值为1.
         * @invariant 硬编码为1。
         *
         * @else
         *
         * @endif
         *
         * @private @memberof Boolean
         * @qualified constexpr
         * */
        static constexpr std::size_t size_ = 1;

        /**
         * @if zh
         *
         * @brief 缓存的已编码数据。
         * @details 该字段用于缓存已编码的数据，以提高序列化的效率。
         *
         * @else
         *
         * @endif
         * */
        mutable std::array<std::byte, size_> data{};

        /**
         * @if zh
         *
         * @brief 以C++类型表示的布尔值。
         * @details 该字段用于存储布尔值。
         *
         * @else
         *
         * @endif
         *
         * @private @memberof Boolean
         * @see value()
         * */
        bool value_;

    public:
        /** @typedef type
         *
         * @if zh
         *
         * @brief 该结构体的原始值类型。
         * @details 该结构体所维护的类型，设计为Boolean的C++表示类型。
         *
         * @else
         *
         *
         * @endif
         *
         * */
        using type = bool;

        /** @typedef encodeType
         *
         * @if zh
         *
         * @brief 该结构体的编码值类型。
         * @details 该结构体编码后的序列类型，且为字段类型的类型依赖接口之一。
         *
         * @else
         *
         * @endif
         * */
        using encodeType = std::array<std::byte, size_>;

        Boolean() = default;

        /**
         * @if zh
         *
         * @brief 构造函数。
         * @details 构造函数，通过布尔值进行初始化。
         * @param value 传入的布尔值。
         *
         * @else
         *
         * @endif
         * */
        Boolean(bool value);

        /**
         * @if zh
         *
         * @brief 获取该字段的字节数。
         * @return 返回该字段的字节数。
         *
         * @else
         *
         * @endif
         *
         * @qualifier constexpr
         * */
        [[nodiscard]] static constexpr std::size_t size();

        /**
         * @if zh
         *
         * @brief 获取该字段的值。
         * @return 返回该字段的C++表示值。
         *
         * @else
         *
         * @endif
         */
        [[nodiscard]] type value() const;

        /**
         * @if zh
         *
         * @brief 获取该字段的编码值。
         * @details 对该字段的值进行编码，缓存后返回编码的数据。
         * @return 返回编码后的序列。
         *
         * @else
         *
         * @endif
         */
        [[nodiscard]] encodeType encode() const;

        /**
         * @if zh
         *
         * @brief 解码函数。
         * @details 将传入的字节数据解码为Boolean字段。
         * @param data 字节数据。
         * @return 数据表示的Boolean类型字段。
         *
         * @else
         *
         * @endif
         */
        static auto decode(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<typename T>
    concept is_boolean_field = std::is_same_v<T, Boolean>;

}  // namespace minecraft::protocol

#include "boolean.hpp"

#endif  // BOOLEAN_H
