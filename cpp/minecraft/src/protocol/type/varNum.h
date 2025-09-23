// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file varNum.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/16 12:42
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef VARNUM_H
#define VARNUM_H
#pragma once

#include <functional>
#include <string>

namespace minecraft::protocol {

    /** @namespace detail
     *
     * @if zh
     * @brief 变长数字实现细节命名空间
     * @details 包含VarInt/VarLong的底层实现和工具函数
     *
     * @else
     * @brief Variable-length number implementation details namespace
     * @details Contains low-level implementation and utility functions for VarInt/VarLong
     *
     * @endif
     */
    namespace detail {
        /**
         * @if zh
         * @brief 数据段位掩码（0x7F）
         * @details 用于提取7位有效数据
         *
         * @else
         * @brief Data segment bit mask (0x7F)
         * @details Used to extract 7-bit valid data
         *
         * @endif
         */
        template<typename T>
        inline constexpr T SEGMENT_BITS{0x7F};

        /**
         * @if zh
         * @brief 继续位标志（0x80）
         * @details 表示后续还有更多字节
         *
         * @else
         * @brief Continue bit flag (0x80)
         * @details Indicates more bytes follow
         *
         * @endif
         */
        template<typename T>
        inline constexpr T CONTINUE_BIT{0x80};

        /**
         * @if zh
         * @brief 整数类型概念约束
         * @details 限制为int或long类型
         *
         * @else
         * @brief Integer type concept constraint
         * @details Restricted to int or long types
         *
         * @endif
         */
        template<typename T>
        concept intOrLong = std::is_same_v<T, int> || std::is_same_v<T, long>;

        /**
         * @if zh
         * @brief 计算变长数字的序列化长度
         * @tparam T 整数类型
         * @param value 整数值
         * @return 序列化所需的字节数
         *
         * @else
         * @brief Calculate serialization length of variable-length number
         * @tparam T Integer type
         * @param value Integer value
         * @return Number of bytes required for serialization
         *
         * @endif
         */
        template<typename T>
        constexpr std::size_t varNumSize(T value);

        /**
         * @if zh
         * @brief 通用变长数字编码模板
         * @tparam T 整数类型
         * @tparam C 容器类型
         * @param value 要编码的整数值
         * @param func 字节处理回调函数
         * @return 编码后的容器
         *
         * @else
         * @brief Generic variable-length number encoding template
         * @tparam T Integer type
         * @tparam C Container type
         * @param value Integer value to encode
         * @param func Byte processing callback function
         * @return Encoded container
         *
         * @endif
         */
        template<intOrLong T, typename C>
        constexpr C varNumEncode(T value, void(func)(C &, std::size_t, std::byte));

        /**
         * @if zh
         * @brief 变长数字解码函数
         * @tparam T 整数类型
         * @param value 字节数据指针
         * @return 解码后的整数值
         *
         * @else
         * @brief Variable-length number decoding function
         * @tparam T Integer type
         * @param value Byte data pointer
         * @return Decoded integer value
         *
         * @endif
         */
        template<intOrLong T>
        T varNumDecode(const std::byte *value);

        /** @struct VarNum
         *
         * @if zh
         * @brief Minecraft协议变长数字模板封装（VarInt/VarLong）
         * @details
         * - 使用7位编码+1位继续标志的变长整数编码方案
         * - 小数值占用更少字节，优化网络传输效率
         * - 支持int和long两种整数类型的变长编码
         * @note 基于Minecraft协议标准的ZigZag变长整数编码
         * @tparam T 整数类型（int或long）
         *
         * @else
         * @brief Minecraft protocol variable-length number template encapsulation (VarInt/VarLong)
         * @details
         * - Uses 7-bit encoding + 1-bit continuation flag variable-length integer encoding scheme
         * - Small values occupy fewer bytes, optimizes network transmission efficiency
         * - Supports variable-length encoding for both int and long integer types
         * @note Based on Minecraft protocol standard ZigZag variable-length integer encoding
         * @tparam T Integer type (int or long)
         *
         * @endif
         */
        template<intOrLong T>
        struct VarNum {
        private:
            /**
             * @if zh
             * @brief 序列化数据缓存
             * @details 存储变长编码后的字节序列
             *
             * @else
             * @brief Serialized data cache
             * @details Stores variable-length encoded byte sequence
             *
             * @endif
             */
            mutable std::vector<std::byte> data;

            /**
             * @if zh
             * @brief 序列化长度
             * @details 动态计算基于数值大小的变长编码长度
             *
             * @else
             * @brief Serialization length
             * @details Dynamically calculated variable-length encoding length based on value size
             *
             * @endif
             */
            std::size_t size_;

            /**
             * @if zh
             * @brief 整数值
             * @details 存储原始的整数值
             *
             * @else
             * @brief Integer value
             * @details Stores original integer value
             *
             * @endif
             */
            T value_;

        public:
            /**
             * @if zh
             * @brief 原始值类型（模板参数T）
             *
             * @else
             * @brief Raw value type (template parameter T)
             *
             * @endif
             */
            using type = T;

            /**
             * @if zh
             * @brief 序列化类型（std::vector<std::byte>）
             * @details 变长字节向量，长度取决于数值大小
             *
             * @else
             * @brief Serialization type (std::vector<std::byte>)
             * @details Variable-length byte vector, length depends on value size
             *
             * @endif
             */
            using encodeType = std::vector<std::byte>;

            /**
             * @if zh
             * @brief 默认构造函数
             * @post value_ = 0, size_ = 1（编码0的VarInt长度）
             *
             * @else
             * @brief Default constructor
             * @post value_ = 0, size_ = 1 (VarInt length for encoding 0)
             *
             * @endif
             */
            VarNum();

            /**
             * @if zh
             * @brief 显式值构造函数
             * @param value 整数值
             * @post 自动计算变长编码长度
             *
             * @else
             * @brief Explicit value constructor
             * @param value Integer value
             * @post Automatically calculates variable-length encoding length
             *
             * @endif
             */
            explicit VarNum(T value);

            VarNum(const VarNum &other) = default;

            VarNum(VarNum &&other) = default;

            VarNum &operator=(const VarNum &other) = default;

            VarNum &operator=(VarNum &&other) = default;

            /**
             * @if zh
             * @brief 获取序列化长度
             * @return 变长编码所需的字节数（1-5字节用于int，1-10字节用于long）
             *
             * @else
             * @brief Get serialization length
             * @return Number of bytes required for variable-length encoding (1-5 bytes for int, 1-10 bytes for long)
             *
             * @endif
             */
            [[nodiscard]] std::size_t size() const;

            /**
             * @if zh
             * @brief 获取整数值
             * @return 原始的整数值
             *
             * @else
             * @brief Get integer value
             * @return Original integer value
             *
             * @endif
             */
            [[nodiscard]] type value() const;

            /**
             * @if zh
             * @brief 序列化为字节数组（变长编码）
             * @details 使用7位分段编码，每个字节最高位表示是否继续
             * @return 变长编码的字节序列
             *
             * @else
             * @brief Serialize to byte array (variable-length encoding)
             * @details Uses 7-bit segment encoding, highest bit of each byte indicates continuation
             * @return Variable-length encoded byte sequence
             *
             * @endif
             */
            [[nodiscard]] encodeType encode() const;

            /**
             * @if zh
             * @brief 从字节数据反序列化
             * @param data 字节数据指针
             * @return 反序列化的VarNum对象
             * @pre data必须包含有效的变长编码数据
             * @throws 当编码溢出时抛出std::runtime_error
             *
             * @else
             * @brief Deserialize from byte data
             * @param data Byte data pointer
             * @return Deserialized VarNum object
             * @pre data must contain valid variable-length encoded data
             * @throws std::runtime_error when encoding overflows
             *
             * @endif
             */
            static auto decode(const std::byte *data);

            /**
             * @if zh
             * @brief 获取可读字符串表示
             * @return 整数的十进制字符串表示
             *
             * @else
             * @brief Get human-readable string representation
             * @return Decimal string representation of integer
             *
             * @endif
             */
            [[nodiscard]] std::string toString() const;

            /**
             * @if zh
             * @brief 获取十六进制表示
             * @return 变长编码数据的十六进制字符串
             *
             * @else
             * @brief Get hexadecimal representation
             * @return Hexadecimal string of variable-length encoded data
             *
             * @endif
             */
            [[nodiscard]] std::string toHexString() const;
        };

    }  // namespace detail

    /**
     * @if zh
     * @brief 变长整数类型别名（VarInt）
     * @details 用于表示-2^31到2^31-1范围的变长编码整数
     *
     * @else
     * @brief Variable-length integer type alias (VarInt)
     * @details Used for variable-length encoded integers in range -2^31 to 2^31-1
     *
     * @endif
     */
    using VarInt = detail::VarNum<int>;

    /** @concept is_var_int_field
     *
     * @if zh
     * @brief 变长整数字段类型概念约束
     * @details 用于模板编程中的变长整数字段类型检查
     * @tparam T 待检查类型
     *
     * @else
     * @brief Variable-length integer field type concept constraint
     * @details Used for variable-length integer field type checking in template programming
     * @tparam T Type to be checked
     *
     * @endif
     */
    template<typename T>
    concept is_var_int_field = std::is_same_v<T, VarInt>;

    /**
     * @if zh
     * @brief 变长长整数类型别名（VarLong）
     * @details 用于表示极大范围的变长编码长整数
     *
     * @else
     * @brief Variable-length long integer type alias (VarLong)
     * @details Used for variable-length encoded long integers with very large range
     *
     * @endif
     */
    using VarLong = detail::VarNum<long>;

    /** @concept is_var_long_field
     *
     * @if zh
     * @brief 变长长整数字段类型概念约束
     * @details 用于模板编程中的变长长整数字段类型检查
     * @tparam T 待检查类型
     *
     * @else
     * @brief Variable-length long integer field type concept constraint
     * @details Used for variable-length long integer field type checking in template programming
     * @tparam T Type to be checked
     *
     * @endif
     */
    template<typename T>
    concept is_var_long_field = std::is_same_v<T, VarLong>;

    /**
     * @if zh
     * @brief 变长数字类型特征检查模板
     * @details 用于编译时判断是否为VarNum模板实例
     *
     * @else
     * @brief Variable-length number type trait check template
     * @details Used for compile-time determination if type is VarNum template instance
     *
     * @endif
     */
    template<typename>
    struct isVarNumField : std::false_type {};

    template<detail::intOrLong T>
    struct isVarNumField<detail::VarNum<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool isVarNumField_v = isVarNumField<T>::value;

    /** @concept is_var_num_field
     *
     * @if zh
     * @brief 变长数字类型概念约束
     * @details 用于模板编程中的变长数字类型检查
     *
     * @else
     * @brief Variable-length number type concept constraint
     * @details Used for variable-length number type checking in template programming
     *
     * @endif
     */
    template<typename T>
    concept is_var_num_field = isVarNumField_v<T>;

}  // namespace minecraft::protocol

#include "varNum.hpp"

#endif  // VARNUM_H
