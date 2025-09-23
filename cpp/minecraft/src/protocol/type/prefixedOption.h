// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file prefixedOption.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/14 14:46
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef PREFIXEDOPTION_H
#define PREFIXEDOPTION_H
#pragma once

#include <optional>
#include <string>
#include <vector>

namespace minecraft::protocol {

    /** @struct PrefixedOption
     *
     * @if zh
     * @brief Minecraft协议带前缀的可选字段封装（Boolean标志 + 可选值）
     * @details
     * - 使用Boolean字段作为存在性标志，后跟可选的实际值
     * - 空值仅占用1字节（Boolean false），有值则占用1+sizeof(T)字节
     * - 提供类型安全的可选值操作和序列化缓存
     * @note 与Option的区别：序列化格式包含显式的Boolean前缀
     * @tparam T 可选值的实际类型
     *
     * @else
     * @brief Minecraft protocol prefixed optional field encapsulation (Boolean flag + optional value)
     * @details
     * - Uses Boolean field as existence flag followed by optional actual value
     * - Empty values occupy only 1 byte (Boolean false), non-empty values occupy 1+sizeof(T) bytes
     * - Provides type-safe optional value operations and serialization caching
     * @note Difference from Option: serialization format includes explicit Boolean prefix
     * @tparam T Actual type of optional value
     *
     * @endif
     */
    template<typename T>
    struct PrefixedOption {
    private:
        /**
         * @if zh
         * @brief 序列化数据缓存
         * @details 存储Boolean标志和可选值的序列化数据
         *
         * @else
         * @brief Serialized data cache
         * @details Stores Boolean flag and optional value serialized data
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
         * @brief 序列化总长度
         * @details 动态计算：Boolean长度 + （值存在时的值长度）
         *
         * @else
         * @brief Total serialization length
         * @details Dynamically calculated: Boolean length + (value length when exists)
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
         * @details 变长字节向量，包含Boolean标志和可选值数据
         *
         * @else
         * @brief Serialization type (std::vector<std::byte>)
         * @details Variable-length byte vector containing Boolean flag and optional value data
         *
         * @endif
         */
        using encodeType = std::vector<std::byte>;

        /**
         * @if zh
         * @brief 默认构造函数（空值）
         * @post value_ = std::nullopt, size_ = Boolean::size()
         *
         * @else
         * @brief Default constructor (empty value)
         * @post value_ = std::nullopt, size_ = Boolean::size()
         *
         * @endif
         */
        PrefixedOption();

        /**
         * @if zh
         * @brief 可选值构造函数
         * @param value 可选值对象
         * @post 自动计算序列化总长度
         *
         * @else
         * @brief Optional value constructor
         * @param value Optional value object
         * @post Automatically calculates total serialization length
         *
         * @endif
         */
        PrefixedOption(const std::optional<T>& value);

        /**
         * @if zh
         * @brief 获取序列化长度
         * @return Boolean长度 + （值存在时的值长度）
         *
         * @else
         * @brief Get serialization length
         * @return Boolean length + (value length when exists)
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
         * @details 先序列化Boolean存在标志，再序列化实际值（如果存在）
         * @return 包含完整序列化数据的字节向量
         *
         * @else
         * @brief Serialize to byte array
         * @details First serializes Boolean existence flag, then serializes actual value (if exists)
         * @return Byte vector containing complete serialized data
         *
         * @endif
         */
        [[nodiscard]] encodeType encode() const;

        /**
         * @if zh
         * @brief 从字节数据反序列化
         * @param data 字节数据指针
         * @return 反序列化的PrefixedOption对象
         * @pre data必须包含有效的Boolean标志和相应数据
         *
         * @else
         * @brief Deserialize from byte data
         * @param data Byte data pointer
         * @return Deserialized PrefixedOption object
         * @pre data must contain valid Boolean flag and corresponding data
         *
         * @endif
         */
        static auto decode(const std::byte* data);

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
         * @return 完整序列化数据的十六进制字符串
         *
         * @else
         * @brief Get hexadecimal representation
         * @return Hexadecimal string of complete serialized data
         *
         * @endif
         */
        [[nodiscard]] std::string toHexString() const;
    };

    /**
     * @if zh
     * @brief 带前缀可选类型特征检查模板
     * @details 用于编译时判断是否为PrefixedOption模板实例
     *
     * @else
     * @brief Prefixed optional type trait check template
     * @details Used for compile-time determination if type is PrefixedOption template instance
     *
     * @endif
     */
    template<typename>
    struct isPrefixedOptionField : std::false_type {};

    template<typename T>
    struct isPrefixedOptionField<PrefixedOption<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool isPrefixedOption_v = isPrefixedOptionField<T>::value;

    /** @concept is_prefixed_option_field
     *
     * @if zh
     * @brief 带前缀可选类型概念约束
     * @details 用于模板编程中的带前缀可选类型检查
     *
     * @else
     * @brief Prefixed optional type concept constraint
     * @details Used for prefixed optional type checking in template programming
     *
     * @endif
     */
    template<typename T>
    concept is_prefixed_option_field = isPrefixedOption_v<T>;

}  // namespace minecraft::protocol

#include "prefixedOption.hpp"

#endif  // PREFIXEDOPTION_H
