// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file prefixedArray.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/13 23:33
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef PREFIXEDARRAY_H
#define PREFIXEDARRAY_H
#pragma once

#include <string>
#include <vector>

namespace minecraft::protocol {

    /** @struct PrefixedArray
     *
     * @if zh
     * @brief Minecraft协议带长度前缀的数组封装（VarInt + 元素序列）
     * @details
     * - 使用VarInt编码数组长度，优化小数组的存储效率
     * - 支持任意协议字段类型作为数组元素
     * - 自动计算和缓存序列化总长度
     * @note 与Array的区别：长度前缀使用VarInt而非固定大小整数
     * @tparam T 数组元素类型（默认为std::byte）
     *
     * @else
     * @brief Minecraft protocol length-prefixed array encapsulation (VarInt + element sequence)
     * @details
     * - Uses VarInt encoding for array length, optimizes storage efficiency for small arrays
     * - Supports arbitrary protocol field types as array elements
     * - Automatically calculates and caches total serialization length
     * @note Difference from Array: length prefix uses VarInt instead of fixed-size integer
     * @tparam T Array element type (defaults to std::byte)
     *
     * @endif
     */
    template<typename T = std::byte>
    struct PrefixedArray {
    private:
        /**
         * @if zh
         * @brief 数组数据值
         * @details 存储实际的数组元素
         *
         * @else
         * @brief Array data value
         * @details Stores actual array elements
         *
         * @endif
         */
        std::vector<T> value_;

        /**
         * @if zh
         * @brief 序列化数据缓存
         * @details 存储长度前缀和所有元素的序列化数据
         *
         * @else
         * @brief Serialized data cache
         * @details Stores length prefix and serialized data of all elements
         *
         * @endif
         */
        mutable std::vector<std::byte> data{};

        /**
         * @if zh
         * @brief 缓存有效性标志
         * @details 数组内容变化时自动失效
         *
         * @else
         * @brief Cache validity flag
         * @details Automatically invalidated when array content changes
         *
         * @endif
         */
        mutable bool cached = false;

        /**
         * @if zh
         * @brief 序列化总长度
         * @details 动态计算：VarInt长度 + 所有元素序列化长度之和
         *
         * @else
         * @brief Total serialization length
         * @details Dynamically calculated: VarInt length + sum of all element serialization lengths
         *
         * @endif
         */
        std::size_t size_;

    public:
        /**
         * @if zh
         * @brief 原始值类型（std::vector<T>）
         *
         * @else
         * @brief Raw value type (std::vector<T>)
         *
         * @endif
         */
        using type = std::vector<T>;

        /**
         * @if zh
         * @brief 序列化类型（std::vector<std::byte>）
         * @details 变长字节向量，包含长度前缀和元素数据
         *
         * @else
         * @brief Serialization type (std::vector<std::byte>)
         * @details Variable-length byte vector containing length prefix and element data
         *
         * @endif
         */
        using encodeType = std::vector<std::byte>;

        /**
         * @if zh
         * @brief 默认构造函数（空数组）
         * @post 创建空数组，size_ = VarInt(0)的长度
         *
         * @else
         * @brief Default constructor (empty array)
         * @post Creates empty array, size_ = length of VarInt(0)
         *
         * @endif
         */
        PrefixedArray();

        /**
         * @if zh
         * @brief 向量构造函数
         * @param value 初始化向量
         * @post 自动计算序列化总长度
         *
         * @else
         * @brief Vector constructor
         * @param value Initialization vector
         * @post Automatically calculates total serialization length
         *
         * @endif
         */
        PrefixedArray(type value);

        /**
         * @if zh
         * @brief 获取序列化总长度
         * @return VarInt长度前缀 + 所有元素序列化长度之和
         *
         * @else
         * @brief Get total serialization length
         * @return VarInt length prefix + sum of all element serialization lengths
         *
         * @endif
         */
        [[nodiscard]] std::size_t size() const;

        /**
         * @if zh
         * @brief 获取数组数据
         * @return 包含所有元素的向量
         *
         * @else
         * @brief Get array data
         * @return Vector containing all elements
         *
         * @endif
         */
        [[nodiscard]] type value() const;

        /**
         * @if zh
         * @brief 序列化为字节数组
         * @details 先序列化VarInt长度前缀，再序列化所有元素
         * @return 包含完整序列化数据的字节向量
         *
         * @else
         * @brief Serialize to byte array
         * @details First serializes VarInt length prefix, then serializes all elements
         * @return Byte vector containing complete serialized data
         *
         * @endif
         */
        [[nodiscard]] encodeType encode() const;

        /**
         * @if zh
         * @brief 从字节数据反序列化
         * @param data 字节数据指针
         * @return 反序列化的PrefixedArray对象
         * @pre data必须包含有效的VarInt长度前缀和相应数量的元素数据
         *
         * @else
         * @brief Deserialize from byte data
         * @param data Byte data pointer
         * @return Deserialized PrefixedArray object
         * @pre data must contain valid VarInt length prefix and corresponding number of element data
         *
         * @endif
         */
        static auto decode(const std::byte* data);

        /**
         * @if zh
         * @brief 获取可读字符串表示
         * @return 格式："[elem1, elem2, ...]"
         * @details 自动处理各元素的字符串表示
         *
         * @else
         * @brief Get human-readable string representation
         * @return Format: "[elem1, elem2, ...]"
         * @details Automatically handles string representation of each element
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
     * @brief 带前缀数组类型特征检查模板
     * @details 用于编译时判断是否为PrefixedArray模板实例
     *
     * @else
     * @brief Prefixed array type trait check template
     * @details Used for compile-time determination if type is PrefixedArray template instance
     *
     * @endif
     */
    template<typename>
    struct isPrefixedArrayField : std::false_type {};

    template<typename T>
    struct isPrefixedArrayField<PrefixedArray<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool isPrefixedArrayField_v = isPrefixedArrayField<T>::value;

    /** @concept is_prefixed_array_field
     *
     * @if zh
     * @brief 带前缀数组类型概念约束
     * @details 用于模板编程中的带前缀数组类型检查
     *
     * @else
     * @brief Prefixed array type concept constraint
     * @details Used for prefixed array type checking in template programming
     *
     * @endif
     */
    template<typename T>
    concept is_prefixed_array_field = isPrefixedArrayField_v<T>;

}  // namespace minecraft::protocol

#include "prefixedArray.hpp"

#endif  // PREFIXEDARRAY_H
