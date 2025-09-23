// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file mcOption.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/14 11:40
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef MCOPTION_H
#define MCOPTION_H
#pragma once

#include <optional>
#include <string>
#include <vector>

namespace minecraft::protocol {

    /** @struct Option
     *
     * @if zh
     * @brief Minecraft协议可选字段封装（类似std::optional）
     * @details
     * - 基于Boolean字段的条件序列化机制
     * - 空值不占用数据空间（仅存在标志位）
     * - 支持任意类型的可选字段封装
     * @note 使用Boolean字段作为存在性标志，实现空间优化
     * @tparam T 可选值的实际类型
     *
     * @else
     * @brief Minecraft protocol optional field encapsulation (similar to std::optional)
     * @details
     * - Conditional serialization mechanism based on Boolean field
     * - Empty values occupy no data space (only existence flag)
     * - Supports optional field encapsulation of arbitrary types
     * @note Uses Boolean field as existence flag, implements space optimization
     * @tparam T Actual type of optional value
     *
     * @endif
     */
    template<typename T>
    struct Option {
    private:
        /**
         * @if zh
         * @brief 序列化数据缓存
         * @details 仅当值存在时存储序列化数据
         *
         * @else
         * @brief Serialized data cache
         * @details Stores serialized data only when value exists
         *
         * @endif
         */
        mutable std::vector<std::byte> data{};

        /**
         * @if zh
         * @brief 缓存有效性标志
         * @details 值变化时自动失效
         *
         * @else
         * @brief Cache validity flag
         * @details Automatically invalidated when value changes
         *
         * @endif
         */
        mutable bool cached = false;

        /**
         * @if zh
         * @brief 序列化数据长度
         * @details 动态计算：值存在时为实际长度，否则为0
         *
         * @else
         * @brief Serialized data length
         * @details Dynamically calculated: actual length when value exists, 0 otherwise
         *
         * @endif
         */
        std::size_t size_;

        /**
         * @if zh
         * @brief 可选值容器
         * @details 使用std::optional存储实际值
         *
         * @else
         * @brief Optional value container
         * @details Uses std::optional to store actual value
         *
         * @endif
         */
        std::optional<T> value_;

    public:
        /**
         * @if zh
         * @brief 原始值类型（std::optional<T>）
         *
         * @else
         * @brief Raw value type (std::optional<T>)
         *
         * @endif
         */
        using type = std::optional<T>;

        /**
         * @if zh
         * @brief 序列化类型（std::vector<std::byte>）
         * @details 变长字节向量，值存在时包含序列化数据
         *
         * @else
         * @brief Serialization type (std::vector<std::byte>)
         * @details Variable-length byte vector, contains serialized data when value exists
         *
         * @endif
         */
        using encodeType = std::vector<std::byte>;

        /**
         * @if zh
         * @brief 默认构造函数（空值）
         * @post value_ = std::nullopt, size_ = 0
         *
         * @else
         * @brief Default constructor (empty value)
         * @post value_ = std::nullopt, size_ = 0
         *
         * @endif
         */
        Option();

        /**
         * @if zh
         * @brief 可选值构造函数
         * @param value 可选值对象
         * @post value_ = value, 自动计算size_
         *
         * @else
         * @brief Optional value constructor
         * @param value Optional value object
         * @post value_ = value, automatically calculates size_
         *
         * @endif
         */
        Option(const std::optional<T>& value);

        /**
         * @if zh
         * @brief 获取序列化长度
         * @return 值存在时的实际长度，否则为0
         *
         * @else
         * @brief Get serialization length
         * @return Actual length when value exists, 0 otherwise
         *
         * @endif
         */
        [[nodiscard]] std::size_t size() const;

        /**
         * @if zh
         * @brief 获取可选值
         * @return 包含值的std::optional对象
         *
         * @else
         * @brief Get optional value
         * @return std::optional object containing value
         *
         * @endif
         */
        [[nodiscard]] type value() const;

        /**
         * @if zh
         * @brief 序列化为字节数组
         * @details 仅当值存在时进行实际序列化
         * @return 值存在时的序列化数据，否则空向量
         *
         * @else
         * @brief Serialize to byte array
         * @details Performs actual serialization only when value exists
         * @return Serialized data when value exists, empty vector otherwise
         *
         * @endif
         */
        [[nodiscard]] encodeType encode() const;

        /**
         * @if zh
         * @brief 从字节数据反序列化
         * @param data 字节数据指针
         * @param boolField Boolean存在性标志字段
         * @return 反序列化的Option对象
         * @pre boolField决定是否从data读取实际值
         *
         * @else
         * @brief Deserialize from byte data
         * @param data Byte data pointer
         * @param boolField Boolean existence flag field
         * @return Deserialized Option object
         * @pre boolField determines whether to read actual value from data
         *
         * @endif
         */
        static auto decode(const std::byte* data, const Boolean& boolField);

        /**
         * @if zh
         * @brief 获取可读字符串表示
         * @return 值存在时的字符串表示，否则"null"
         *
         * @else
         * @brief Get human-readable string representation
         * @return String representation when value exists, "null" otherwise
         *
         * @endif
         */
        [[nodiscard]] std::string toString() const;

        /**
         * @if zh
         * @brief 获取十六进制表示
         * @return 值存在时的十六进制字符串，否则空字符串
         *
         * @else
         * @brief Get hexadecimal representation
         * @return Hexadecimal string when value exists, empty string otherwise
         *
         * @endif
         */
        [[nodiscard]] std::string toHexString() const;
    };

    /**
     * @if zh
     * @brief 可选类型特征检查模板
     * @details 用于编译时判断是否为Option模板实例
     *
     * @else
     * @brief Optional type trait check template
     * @details Used for compile-time determination if type is Option template instance
     *
     * @endif
     */
    template<typename>
    struct isOptionField : std::false_type {};

    template<typename T>
    struct isOptionField<Option<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool isOptionField_v = isOptionField<T>::value;

    /** @concept is_option_field
     *
     * @if zh
     * @brief 可选类型概念约束
     * @details 用于模板编程中的可选类型约束
     *
     * @else
     * @brief Optional type concept constraint
     * @details Used for optional type constraints in template programming
     *
     * @endif
     */
    template<typename T>
    concept is_option_field = isOptionField_v<T>;

}  // namespace minecraft::protocol

#include "mcOption.hpp"

#endif  // MCOPTION_H
