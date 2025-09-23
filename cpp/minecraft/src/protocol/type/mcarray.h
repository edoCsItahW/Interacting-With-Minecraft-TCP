// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file mcarray.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/11 18:15
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef MCARRAY_H
#define MCARRAY_H
#pragma once

#include "varNum.h"
#include <string>
#include <vector>

namespace minecraft::protocol {

    /** @struct Array
     *
     * @if zh
     * @brief Minecraft协议动态数组字段封装
     * @details
     * - 支持任意元素类型的动态数组序列化
     * - 自动处理元素类型的序列化方法调用
     * - 使用VarInt编码数组长度，优化小数组存储
     * @note 模板参数T决定数组元素类型，默认为std::byte
     * @tparam T 数组元素类型（支持协议字段类型或基本类型）
     *
     * @else
     * @brief Minecraft protocol dynamic array field encapsulation
     * @details
     * - Supports dynamic array serialization of arbitrary element types
     * - Automatically handles element type serialization method calls
     * - Uses VarInt encoding for array length, optimizes small array storage
     * @note Template parameter T determines array element type, defaults to std::byte
     * @tparam T Array element type (supports protocol field types or basic types)
     *
     * @endif
     */
    template<typename T = std::byte>
    struct Array {
    private:
        /**
         * @if zh
         * @brief 数组元素数量
         * @details 动态记录当前数组包含的元素个数
         *
         * @else
         * @brief Number of array elements
         * @details Dynamically records current number of elements in array
         *
         * @endif
         */
        std::size_t size_ = 0;

        /**
         * @if zh
         * @brief 数组数据值
         * @details 使用std::vector存储实际数组元素
         *
         * @else
         * @brief Array data value
         * @details Uses std::vector to store actual array elements
         *
         * @endif
         */
        std::vector<T> value_;

        /**
         * @if zh
         * @brief 缓存有效性标志
         * @details 数组内容修改时自动失效
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
         * @brief 序列化数据缓存
         * @details 存储所有元素的序列化结果拼接
         *
         * @else
         * @brief Serialized data cache
         * @details Stores concatenated serialization results of all elements
         *
         * @endif
         */
        mutable std::vector<std::byte> data{};

        /**
         * @if zh
         * @brief 向量构造函数（私有）
         * @param value 初始化向量
         * @post value_ = value, size_ = value.size()
         *
         * @else
         * @brief Vector constructor (private)
         * @param value Initialization vector
         * @post value_ = value, size_ = value.size()
         *
         * @endif
         */
        Array(const std::vector<T>& value);

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
         * @details 变长字节向量，包含所有元素的序列化数据
         *
         * @else
         * @brief Serialization type (std::vector<std::byte>)
         * @details Variable-length byte vector containing serialized data of all elements
         *
         * @endif
         */
        using encodeType = std::vector<std::byte>;

        Array() = default;

        /**
         * @if zh
         * @brief 获取数组元素数量
         * @return 当前数组包含的元素个数
         *
         * @else
         * @brief Get number of array elements
         * @return Current number of elements in array
         *
         * @endif
         */
        [[nodiscard]] std::size_t size() const;

        /**
         * @if zh
         * @brief 获取数组数据
         * @return 包含所有元素的向量引用
         *
         * @else
         * @brief Get array data
         * @return Reference to vector containing all elements
         *
         * @endif
         */
        [[nodiscard]] type value() const;

        /**
         * @if zh
         * @brief 序列化为字节数组
         * @details 递归序列化每个元素并拼接结果
         * @return 包含所有元素序列化数据的字节向量
         * @post cached = true（如果之前为false）
         *
         * @else
         * @brief Serialize to byte array
         * @details Recursively serializes each element and concatenates results
         * @return Byte vector containing serialized data of all elements
         * @post cached = true (if previously false)
         *
         * @endif
         */
        [[nodiscard]] encodeType encode() const;

        /**
         * @if zh
         * @brief 从字节数据反序列化（使用VarInt长度字段）
         * @param data 字节数据指针
         * @param sizeField VarInt编码的数组长度字段
         * @return 反序列化的Array对象
         * @pre data必须包含足够长度的有效数据
         *
         * @else
         * @brief Deserialize from byte data (using VarInt length field)
         * @param data Byte data pointer
         * @param sizeField VarInt encoded array length field
         * @return Deserialized Array object
         * @pre data must contain sufficient valid data
         *
         * @endif
         */
        static auto decode(const std::byte* data, const VarInt& sizeField);

        /**
         * @if zh
         * @brief 从字节数据反序列化（使用显式长度）
         * @param data 字节数据指针
         * @param size 数组元素数量
         * @return 反序列化的Array对象
         * @note 直接指定长度，避免VarInt解码开销
         *
         * @else
         * @brief Deserialize from byte data (using explicit length)
         * @param data Byte data pointer
         * @param size Number of array elements
         * @return Deserialized Array object
         * @note Directly specifies length, avoids VarInt decoding overhead
         *
         * @endif
         */
        static auto decode(const std::byte* data, std::size_t size);

        /**
         * @if zh
         * @brief 获取可读字符串表示
         * @return 格式："[elem1, elem2, ...]"
         * @details 自动调用元素的toString方法或适当转换
         *
         * @else
         * @brief Get human-readable string representation
         * @return Format: "[elem1, elem2, ...]"
         * @details Automatically calls element's toString method or appropriate conversion
         *
         * @endif
         */
        [[nodiscard]] std::string toString() const;

        /**
         * @if zh
         * @brief 获取十六进制表示
         * @return 整个数组序列化数据的十六进制字符串
         *
         * @else
         * @brief Get hexadecimal representation
         * @return Hexadecimal string of entire array's serialized data
         *
         * @endif
         */
        [[nodiscard]] std::string toHexString() const;
    };

    /**
     * @if zh
     * @brief 数组类型特征检查模板
     * @details 用于编译时判断是否为Array模板实例
     *
     * @else
     * @brief Array type trait check template
     * @details Used for compile-time determination if type is Array template instance
     *
     * @endif
     */
    template<typename>
    struct isArrayField : std::false_type {};

    template<typename T>
    struct isArrayField<Array<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool isArrayField_v = isArrayField<T>::value;

    /** @concept is_array_field
     *
     * @if zh
     * @brief 数组类型概念约束
     * @details 用于模板编程中的数组类型约束
     *
     * @else
     * @brief Array type concept constraint
     * @details Used for array type constraints in template programming
     *
     * @endif
     */
    template<typename T>
    concept is_array_field = isArrayField_v<T>;

}  // namespace minecraft::protocol

#include "mcarray.hpp"

#endif  // MCARRAY_H
