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

    /** @struct CompoundArray
     *
     * @if zh
     * @brief Minecraft协议复合数组字段封装（支持多类型元组序列化）
     * @details
     * - 使用变长模板参数支持任意类型组合的元组序列化
     * - 自动处理各字段的序列化/反序列化方法调用
     * - 提供序列化缓存机制和可读字符串表示
     * @tparam Ts 元组元素类型列表
     * @note 线程安全：const方法线程安全，非const方法需外部同步
     *
     * @else
     * @brief Minecraft protocol compound array field encapsulation (supports multi-type tuple serialization)
     * @details
     * - Uses variadic templates to support tuple serialization of arbitrary type combinations
     * - Automatically handles serialization/deserialization method calls for each field
     * - Provides serialization caching and human-readable string representation
     * @tparam Ts Tuple element type list
     * @note Thread safety: const methods are thread-safe, non-const methods require external synchronization
     *
     * @endif
     * */
    template<typename... Ts>
    struct CompoundArray {
    private:
        /**
         * @if zh
         * @brief 序列化数据总长度（字节）
         * @details 动态计算各字段序列化长度之和
         *
         * @else
         * @brief Total serialized data length (bytes)
         * @details Dynamically calculates sum of each field's serialization length
         *
         * @endif
         */
        std::size_t size_ = 0;

        /**
         * @if zh
         * @brief 复合数据值（元组存储）
         * @details 存储各类型字段的实际值
         *
         * @else
         * @brief Compound data value (tuple storage)
         * @details Stores actual values of each type field
         *
         * @endif
         */
        std::tuple<Ts...> value_;

        /**
         * @if zh
         * @brief 缓存有效性标志
         * @details value_修改时设置为false，序列化时设置为true
         * @note mutable允许const方法修改（逻辑恒定性）
         *
         * @else
         * @brief Cache validity flag
         * @details Set to false when value_ changes, set to true after serialization
         * @note mutable enables modification by const methods (logical constness)
         *
         * @endif
         */
        mutable bool cached = false;

        /**
         * @if zh
         * @brief 序列化数据缓存
         * @details 避免重复序列化，修改value_后自动失效
         * @note mutable允许const方法修改缓存（逻辑恒定性）
         *
         * @else
         * @brief Serialized data cache
         * @details Avoids repeated serialization, invalidated when value_ changes
         * @note mutable enables modification by const methods (logical constness)
         *
         * @endif
         */
        mutable std::vector<std::byte> data{};

        /**
         * @if zh
         * @brief 元组构造函数
         * @param value 右值元组
         * @post value_ = std::move(value)
         *
         * @else
         * @brief Tuple constructor
         * @param value Rvalue tuple
         * @post value_ = std::move(value)
         *
         * @endif
         */
        CompoundArray(std::tuple<Ts...>&& value);

    public:
        /**
         * @if zh
         * @brief 原始值类型（std::tuple<Ts...>）
         *
         * @else
         * @brief Raw value type (std::tuple<Ts...>)
         *
         * @endif
         */
        using type = std::tuple<Ts...>;

        /**
         * @if zh
         * @brief 序列化类型（std::vector<std::byte>）
         *
         * @else
         * @brief Serialization type (std::vector<std::byte>)
         *
         * @endif
         */
        using encodeType = std::vector<std::byte>;

        CompoundArray() = default;

        /**
         * @if zh
         * @brief 可变参数构造函数
         * @param args 构造参数
         * @post value_ = std::tuple{std::forward<Ts>(args)...}
         *
         * @else
         * @brief Variadic constructor
         * @param args Construction parameters
         * @post value_ = std::tuple{std::forward<Ts>(args)...}
         *
         * @endif
         */
        CompoundArray(Ts&&... args);

        /**
         * @if zh
         * @brief 获取序列化长度
         * @return 各字段序列化长度之和
         *
         * @else
         * @brief Get serialization length
         * @return Sum of each field's serialization length
         *
         * @endif
         */
        [[nodiscard]] std::size_t size() const;

        /**
         * @if zh
         * @brief 获取复合数据值
         * @return 包含各字段值的元组
         *
         * @else
         * @brief Get compound data value
         * @return Tuple containing each field's value
         *
         * @endif
         */
        [[nodiscard]] type value() const;

        /**
         * @if zh
         * @brief 序列化为字节数组
         * @details 递归处理各字段序列化并拼接结果
         * @return 包含所有字段序列化数据的字节向量
         * @post cached = true（如果之前为false）
         *
         * @else
         * @brief Serialize to byte array
         * @details Recursively processes each field's serialization and concatenates results
         * @return Byte vector containing all fields' serialized data
         * @post cached = true (if previously false)
         *
         * @endif
         */
        [[nodiscard]] encodeType encode() const;

        /**
         * @if zh
         * @brief 从字节数据反序列化
         * @param data 字节数据指针
         * @return 反序列化的CompoundArray对象
         * @pre data 必须包含足够长度的有效数据
         *
         * @else
         * @brief Deserialize from byte data
         * @param data Byte data pointer
         * @return Deserialized CompoundArray object
         * @pre data must contain sufficient valid data
         *
         * @endif
         */
        static auto decode(const std::byte* data);

        /**
         * @if zh
         * @brief 获取可读字符串表示
         * @return 格式："[value1, value2, ...]"
         *
         * @else
         * @brief Get human-readable string representation
         * @return Format: "[value1, value2, ...]"
         *
         * @endif
         */
        [[nodiscard]] std::string toString() const;

        /**
         * @if zh
         * @brief 获取十六进制表示
         * @return 整个复合数组的十六进制字符串
         *
         * @else
         * @brief Get hexadecimal representation
         * @return Hexadecimal string of entire compound array
         *
         * @endif
         */
        [[nodiscard]] std::string toHexString() const;
    };

    /** @struct isCompoundArrayField
     *
     * @if zh
     * @brief 类型特征检查：是否为CompoundArray类型
     *
     * @else
     * @brief Type trait check: whether is CompoundArray type
     *
     * @endif
     */
    template<typename>
    struct isCompoundArrayField : std::false_type {};

    template<typename... Ts>
    struct isCompoundArrayField<CompoundArray<Ts...>> : std::true_type {};

    template<typename T>
    inline constexpr bool isCompoundArray_v = isCompoundArrayField<T>::value;

    /** @concept is_compound_array_field
     *
     * @if zh
     * @brief 概念检查：是否为CompoundArray类型
     * @tparam T 待检查类型
     *
     * @else
     * @brief Concept check: whether is CompoundArray type
     * @tparam T Type to check
     *
     * @endif
     */
    template<typename T>
    concept is_compound_array_field = isCompoundArray_v<T>;

}  // namespace minecraft::protocol

#include "compoundArray.hpp"

#endif  // COMPOUNDARRAY_H
