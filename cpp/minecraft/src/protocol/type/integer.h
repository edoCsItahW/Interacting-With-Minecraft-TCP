// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file integer.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/17 10:51
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef INTEGER_H
#define INTEGER_H
#pragma once

#include <array>
#include <string>

namespace minecraft::protocol {

    /** @namespace detail
     *
     * @if zh
     * @brief 整数类型模板实现细节命名空间
     * @details 包含通用的整数类型模板实现，支持所有标准整数类型
     *
     * @else
     * @brief Integer type template implementation details namespace
     * @details Contains generic integer type template implementation supporting all standard integer types
     *
     * @endif
     */
    namespace detail {

        /** @concept integer
         *
         * @if zh
         * @brief 整数类型概念约束
         * @details 确保模板参数为标准的整数类型
         *
         * @else
         * @brief Integer type concept constraint
         * @details Ensures template parameter is a standard integer type
         *
         * @endif
         */
        template<typename T>
        concept integer = std::is_integral_v<T>;

        /** @struct Integer
         *
         * @if zh
         * @brief Minecraft协议通用整数类型模板封装
         * @details
         * - 支持所有标准整数类型（有符号/无符号，8/16/32/64位）
         * - 自动处理主机字节序到网络大端序的转换
         * - 提供类型安全的整数操作和序列化缓存
         * @note 模板参数T决定整数大小和符号特性
         * @tparam T 整数类型（int8_t, uint16_t, int32_t等）
         *
         * @else
         * @brief Minecraft protocol generic integer type template encapsulation
         * @details
         * - Supports all standard integer types (signed/unsigned, 8/16/32/64-bit)
         * - Automatically handles host endianness to network big-endian conversion
         * - Provides type-safe integer operations and serialization caching
         * @note Template parameter T determines integer size and signedness
         * @tparam T Integer type (int8_t, uint16_t, int32_t, etc.)
         *
         * @endif
         */
        template<typename T>
        struct Integer {
        private:
            /**
             * @if zh
             * @brief 序列化数据缓存
             * @details 存储大端序表示的整数值，避免重复转换
             * @note 缓存大小由sizeof(T)动态确定
             *
             * @else
             * @brief Serialized data cache
             * @details Stores big-endian representation of integer value, avoids repeated conversion
             * @note Cache size dynamically determined by sizeof(T)
             *
             * @endif
             */
            mutable std::array<std::byte, sizeof(T)> data;

            /**
             * @if zh
             * @brief 缓存有效性标志
             * @details 数值变化时自动失效，确保数据一致性
             *
             * @else
             * @brief Cache validity flag
             * @details Automatically invalidated when value changes, ensures data consistency
             *
             * @endif
             */
            mutable bool cached = false;

            /**
             * @if zh
             * @brief 序列化字节长度
             * @invariant 恒等于sizeof(T)，反映整数类型的实际大小
             *
             * @else
             * @brief Serialized byte length
             * @invariant Always equals sizeof(T), reflects actual size of integer type
             *
             * @endif
             */
            static constexpr auto size_ = sizeof(T);

            /**
             * @if zh
             * @brief 整数值
             * @details 存储原始的整数值，支持所有整数运算
             *
             * @else
             * @brief Integer value
             * @details Stores original integer value, supports all integer operations
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
             * @brief 序列化类型（std::array<std::byte, sizeof(T)>）
             *
             * @else
             * @brief Serialization type (std::array<std::byte, sizeof(T)>)
             *
             * @endif
             */
            using encodeType = std::array<std::byte, size_>;

            /**
             * @if zh
             * @brief 默认构造函数
             * @post value_ = 0, cached = false
             *
             * @else
             * @brief Default constructor
             * @post value_ = 0, cached = false
             *
             * @endif
             */
            Integer();

            /**
             * @if zh
             * @brief 显式值构造函数
             * @param value 整数值
             * @post value_ = value, cached = false
             *
             * @else
             * @brief Explicit value constructor
             * @param value Integer value
             * @post value_ = value, cached = false
             *
             * @endif
             */
            explicit Integer(T value);

            Integer(const Integer &other) = default;

            Integer(Integer &&other) = default;

            Integer &operator=(const Integer &other) = default;

            Integer &operator=(Integer &&other) = default;

            /**
             * @if zh
             * @brief 获取序列化长度
             * @return sizeof(T)字节
             * @note 编译时常量，支持各种整数大小
             *
             * @else
             * @brief Get serialization length
             * @return sizeof(T) bytes
             * @note Compile-time constant, supports various integer sizes
             *
             * @endif
             */
            [[nodiscard]] static constexpr std::size_t size();

            /**
             * @if zh
             * @brief 获取整数值
             * @return 当前存储的整数值
             * @note 直接访问，不涉及字节序转换
             *
             * @else
             * @brief Get integer value
             * @return Currently stored integer value
             * @note Direct access, no endianness conversion involved
             *
             * @endif
             */
            [[nodiscard]] type value() const;

            /**
             * @if zh
             * @brief 序列化为字节数组（大端序）
             * @details 自动处理字节序转换，支持缓存优化
             * @return 大端序表示的整数字节数组
             * @throws 可能抛出不支持的字节序异常
             *
             * @else
             * @brief Serialize to byte array (big-endian)
             * @details Automatically handles endianness conversion, supports caching optimization
             * @return Big-endian representation of integer as byte array
             * @throws May throw unsupported endianness exception
             *
             * @endif
             */
            [[nodiscard]] encodeType encode() const;

            /**
             * @if zh
             * @brief 从字节数据反序列化
             * @param data 字节数据指针（必须包含至少sizeof(T)字节）
             * @return 反序列化的Integer对象
             * @pre data != nullptr且指向足够长度的有效数据
             * @note 自动处理大端序到主机字节序的转换
             *
             * @else
             * @brief Deserialize from byte data
             * @param data Byte data pointer (must contain at least sizeof(T) bytes)
             * @return Deserialized Integer object
             * @pre data != nullptr and points to sufficient valid data
             * @note Automatically handles big-endian to host endianness conversion
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
             * @brief 获取二进制表示
             * @return 整数的二进制位字符串表示
             * @note 不同于其他类型的十六进制表示
             *
             * @else
             * @brief Get binary representation
             * @return Binary bit string representation of integer
             * @note Different from hexadecimal representation of other types
             *
             * @endif
             */
            [[nodiscard]] std::string toHexString() const;
        };

    }  // namespace detail

    /** @typedef Byte
     *
     * @if zh
     * @brief 8位有符号整数类型别名（Byte）
     * @details 用于表示-128到127范围的整数
     *
     * @else
     * @brief 8-bit signed integer type alias (Byte)
     * @details Used for integers in range -128 to 127
     *
     * @endif
     */
    using Byte = detail::Integer<int8_t>;

    /** @concept is_byte_field
     *
     * @if zh
     * @brief 字节类型概念约束
     * @details 用于模板编程中的字节类型约束
     * @tparam T 待检查类型
     *
     * @else
     * @brief Byte type concept constraint
     * @details Used for byte type constraint in template programming
     * @tparam T Type to check
     *
     * @endif
     */
    template<typename T>
    concept is_byte_field = std::is_same_v<T, Byte>;

    /** @typedef Short
     *
     * @if zh
     * @brief 16位有符号整数类型别名（Short）
     * @details 用于表示-32768到32767范围的整数
     *
     * @else
     * @brief 16-bit signed integer type alias (Short)
     * @details Used for integers in range -32768 to 32767
     *
     * @endif
     */
    using Short = detail::Integer<int16_t>;

    /** @concept is_short_field
     *
     * @if zh
     * @brief 短整数类型概念约束
     * @details 用于模板编程中的短整数类型约束
     * @tparam T 待检查类型
     *
     * @else
     * @brief Short integer type concept constraint
     * @details Used for short integer type constraint in template programming
     * @tparam T Type to check
     */
    template<typename T>
    concept is_short_field = std::is_same_v<T, Short>;

    /** @typedef Int
     *
     * @if zh
     * @brief 32位有符号整数类型别名（Int）
     * @details 用于表示约±21亿范围的整数
     *
     * @else
     * @brief 32-bit signed integer type alias (Int)
     * @details Used for integers in range approx. ±2.1 billion
     *
     * @endif
     */
    using Int = detail::Integer<int32_t>;

    template<typename T>
    concept is_int_field = std::is_same_v<T, Int>;

    /** @typedef Long
     *
     * @if zh
     * @brief 64位有符号整数类型别名（Long）
     * @details 用于表示极大范围的整数
     * @tparam T 待检查类型
     *
     * @else
     * @brief 64-bit signed integer type alias (Long)
     * @details Used for very large range integers
     * @tparam T Type to check
     *
     * @endif
     */
    using Long = detail::Integer<int64_t>;

    /** @concept is_long_field
     *
     * @if zh
     * @brief 长整数类型概念约束
     * @details 用于模板编程中的长整数类型约束
     * @tparam T 待检查类型
     *
     * @else
     * @brief Long integer type concept constraint
     * @details Used for long integer type constraint in template programming
     * @tparam T Type to check
     *
     * @endif
     */
    template<typename T>
    concept is_long_field = std::is_same_v<T, Long>;

    /** @typedef UByte
     *
     * @if zh
     * @brief 8位无符号整数类型别名（UByte）
     * @details 用于表示0到255范围的整数
     *
     * @else
     * @brief 8-bit unsigned integer type alias (UByte)
     * @details Used for integers in range 0 to 255
     *
     * @endif
     */
    using UByte = detail::Integer<uint8_t>;

    /** @concept is_ubyte_field
     *
     * @if zh
     * @brief 无符号字节类型概念约束
     * @details 用于模板编程中的无符号字节类型约束
     * @tparam T 待检查类型
     *
     * @else
     * @brief Unsigned byte type concept constraint
     * @details Used for unsigned byte type constraint in template programming
     * @tparam T Type to check
     *
     * @endif
     */
    template<typename T>
    concept is_ubyte_field = std::is_same_v<T, UByte>;

    /** @typedef UShort
     *
     * @if zh
     * @brief 16位无符号整数类型别名（UShort）
     * @details 用于表示0到65535范围的整数
     *
     * @else
     * @brief 16-bit unsigned integer type alias (UShort)
     * @details Used for integers in range 0 to 65535
     *
     * @endif
     */
    using UShort = detail::Integer<uint16_t>;

    /** @concept is_ushort_field
     *
     * @if zh
     * @brief 无符号短整数类型概念约束
     * @details 用于模板编程中的无符号短整数类型约束
     * @tparam T 待检查类型
     *
     * @else
     * @brief Unsigned short integer type concept constraint
     * @details Used for unsigned short integer type constraint in template programming
     * @tparam T Type to check
     *
     * @endif
     */
    template<typename T>
    concept is_ushort_field = std::is_same_v<T, UShort>;

    /** @typedef UInt
     *
     * @if zh
     * @brief 32位无符号整数类型别名（UInt）
     * @details 用于表示0到约42亿范围的整数
     *
     * @else
     * @brief 32-bit unsigned integer type alias (UInt)
     * @details Used for integers in range 0 to approx. 4.2 billion
     *
     * @endif
     */
    using UInt = detail::Integer<uint32_t>;

    /** @concept is_uint_field
     *
     * @if zh
     * @brief 无符号整数类型概念约束
     * @details 用于模板编程中的无符号整数类型约束
     * @tparam T 待检查类型
     *
     * @else
     * @brief Unsigned integer type concept constraint
     * @details Used for unsigned integer type constraint in template programming
     * @tparam T Type to check
     *
     * @endif
     */
    template<typename T>
    concept is_uint_field = std::is_same_v<T, UInt>;

    /** @typedef ULong
     *
     * @if zh
     * @brief 64位无符号整数类型别名（ULong）
     * @details 用于表示极大范围的无符号整数
     *
     * @else
     * @brief 64-bit unsigned integer type alias (ULong)
     * @details Used for very large range unsigned integers
     *
     * @endif
     */
    using ULong = detail::Integer<uint64_t>;

    /** @concept is_ulong_field
     *
     * @if zh
     * @brief 无符号长整数类型概念约束
     * @details 用于模板编程中的无符号长整数类型约束
     * @tparam T 待检查类型
     *
     * @else
     * @brief Unsigned long integer type concept constraint
     * @details Used for unsigned long integer type constraint in template programming
     * @tparam T Type to check
     *
     * @endif
     */
    template<typename T>
    concept is_ulong_field = std::is_same_v<T, ULong>;

    /** @struct isIntegerField
     *
     * @if zh
     * @brief 整数类型特征检查模板
     * @details 用于编译时判断是否为Integer模板实例
     *
     * @else
     * @brief Integer type trait check template
     * @details Used for compile-time determination if type is Integer template instance
     *
     * @endif
     */
    template<typename>
    struct isIntegerField : std::false_type {};

    template<typename T>
    struct isIntegerField<detail::Integer<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool isIntegerField_v = isIntegerField<T>::value;

    /** @concept is_integer_field
     *
     * @if zh
     * @brief 整数类型概念约束
     * @details 用于模板编程中的整数类型约束
     *
     * @else
     * @brief Integer type concept constraint
     * @details Used for integer type constraints in template programming
     *
     * @endif
     */
    template<typename T>
    concept is_integer_field = isIntegerField_v<T>;

}  // namespace minecraft::protocol

#include "integer.hpp"

#endif  // INTEGER_H
