// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file mcuuid.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 00:05
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef MCUUID_H
#define MCUUID_H
#pragma once

#include "../../utils/fstr.h"
#include <string>

namespace minecraft::protocol {

    /** @namespace detail
     *
     * @if zh
     * @brief UUID实现细节命名空间
     * @details 包含UUID生成和转换的内部实现
     *
     * @else
     * @brief UUID implementation details namespace
     * @details Contains internal implementation of UUID generation and conversion
     *
     * @endif
     */
    namespace detail {
        /**
         * @if zh
         * @brief Java风格UUID生成函数
         * @tparam T 字符串类型
         * @param str 输入字符串
         * @return 16字节UUID数组
         * @details 使用MD5哈希生成版本3 UUID
         *
         * @else
         * @brief Java-style UUID generation function
         * @tparam T String type
         * @param str Input string
         * @return 16-byte UUID array
         * @details Generates version 3 UUID using MD5 hash
         *
         * @endif
         */
        template<typename T>
        constexpr auto javaUUID(T str);

        /**
         * @if zh
         * @brief UUID字节数组转标准字符串
         * @param uuidBytes 16字节UUID数组
         * @return 标准UUID格式字符串（8-4-4-4-12）
         *
         * @else
         * @brief Convert UUID byte array to standard string
         * @param uuidBytes 16-byte UUID array
         * @return Standard UUID format string (8-4-4-4-12)
         *
         * @endif
         */
        std::string uuidToString(const std::array<std::byte, 16>& uuidBytes);
    }  // namespace detail

    /** @struct UUID
     *
     * @if zh
     * @brief Minecraft协议通用唯一标识符封装（16字节）
     * @details
     * - 遵循RFC 4122标准的版本3/4 UUID格式
     * - 支持从字符串生成确定性UUID（基于MD5哈希）
     * - 提供标准UUID字符串格式化和解析
     * @note 主要用于实体、玩家、物品等唯一标识
     * @see RFC 4122 UUID标准规范
     *
     * @else
     * @brief Minecraft protocol universally unique identifier encapsulation (16 bytes)
     * @details
     * - Complies with RFC 4122 standard version 3/4 UUID format
     * - Supports deterministic UUID generation from strings (MD5-based)
     * - Provides standard UUID string formatting and parsing
     * @note Mainly used for unique identification of entities, players, items, etc.
     * @see RFC 4122 UUID standard specification
     *
     * @endif
     */
    struct UUID {
    private:
        /**
         * @if zh
         * @brief 序列化字节长度（固定16字节）
         * @invariant 恒为16，符合UUID标准长度
         *
         * @else
         * @brief Serialized byte length (fixed 16 bytes)
         * @invariant Always 16, conforms to UUID standard length
         *
         * @endif
         */
        static constexpr std::size_t size_ = 16;

        /**
         * @if zh
         * @brief UUID原始字节值
         * @details 存储16字节的UUID原始数据
         *
         * @else
         * @brief UUID raw byte value
         * @details Stores 16-byte UUID raw data
         *
         * @endif
         */
        std::array<std::byte, size_> value_;

    public:
        /**
         * @if zh
         * @brief 原始值类型（std::array<std::byte, 16>）
         *
         * @else
         * @brief Raw value type (std::array<std::byte, 16>)
         *
         * @endif
         */
        using type = std::array<std::byte, size_>;

        /**
         * @if zh
         * @brief 序列化类型（std::array<std::byte, 16>）
         * @details UUID本身即为序列化格式，无需转换
         *
         * @else
         * @brief Serialization type (std::array<std::byte, 16>)
         * @details UUID itself is the serialization format, no conversion needed
         *
         * @endif
         */
        using encodeType = std::array<std::byte, size_>;

        /**
         * @if zh
         * @brief 默认构造函数
         * @post 创建全零UUID（nil UUID）
         *
         * @else
         * @brief Default constructor
         * @post Creates all-zero UUID (nil UUID)
         *
         * @endif
         */
        UUID();

        /**
         * @if zh
         * @brief 字节数组构造函数
         * @param value 16字节UUID数据
         * @post value_ = value
         *
         * @else
         * @brief Byte array constructor
         * @param value 16-byte UUID data
         * @post value_ = value
         *
         * @endif
         */
        UUID(const std::array<std::byte, size_>& value);

        UUID(const UUID&) = default;

        UUID(UUID&&) = default;

        UUID& operator=(const UUID&) = default;

        UUID& operator=(UUID&&) = default;

        /**
         * @if zh
         * @brief 获取序列化长度
         * @return 固定16字节
         * @note 全球唯一标识符的标准长度
         *
         * @else
         * @brief Get serialization length
         * @return Fixed 16 bytes
         * @note Standard length for globally unique identifiers
         *
         * @endif
         */
        [[nodiscard]] static constexpr std::size_t size();

        /**
         * @if zh
         * @brief 获取UUID原始字节
         * @return 16字节UUID数组
         *
         * @else
         * @brief Get UUID raw bytes
         * @return 16-byte UUID array
         *
         * @endif
         */
        [[nodiscard]] type value() const;

        /**
         * @if zh
         * @brief 序列化为字节数组
         * @return UUID原始字节数组（直接返回，无转换）
         * @note UUID格式本身即为网络字节序
         *
         * @else
         * @brief Serialize to byte array
         * @return UUID raw byte array (direct return, no conversion)
         * @note UUID format itself is network byte order
         *
         * @endif
         */
        [[nodiscard]] encodeType encode() const;

        /**
         * @if zh
         * @brief 从字节数据反序列化
         * @param data 字节数据指针（必须包含至少16字节）
         * @return 反序列化的UUID对象
         * @pre data != nullptr且指向至少16字节有效数据
         *
         * @else
         * @brief Deserialize from byte data
         * @param data Byte data pointer (must contain at least 16 bytes)
         * @return Deserialized UUID object
         * @pre data != nullptr and points to at least 16 valid bytes
         *
         * @endif
         */
        static auto decode(const std::byte* data);

        /**
         * @if zh
         * @brief 获取标准UUID字符串表示
         * @return 格式："xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
         * @details 符合RFC 4122标准格式
         *
         * @else
         * @brief Get standard UUID string representation
         * @return Format: "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
         * @details Complies with RFC 4122 standard format
         *
         * @endif
         */
        [[nodiscard]] std::string toString() const;

        /**
         * @if zh
         * @brief 获取十六进制表示
         * @return 32字符十六进制字符串（无连字符）
         *
         * @else
         * @brief Get hexadecimal representation
         * @return 32-character hexadecimal string (without hyphens)
         *
         * @endif
         */
        [[nodiscard]] std::string toHexString() const;
    };

    /**
     * @if zh
     * @brief 通用UUID生成函数
     * @tparam T 字符串类型
     * @param str 输入字符串
     * @return UUID对象
     * @details 基于输入字符串生成确定性UUID
     *
     * @else
     * @brief General UUID generation function
     * @tparam T String type
     * @param str Input string
     * @return UUID object
     * @details Generates deterministic UUID based on input string
     *
     * @endif
     */
    template<typename T>
    auto genUUID(T str);

    /**
     * @if zh
     * @brief 编译时常量字符串UUID生成（模板特化）
     * @tparam N 字符串长度
     * @param str 编译时常量字符串
     * @return 编译时计算的UUID对象
     *
     * @else
     * @brief Compile-time constant string UUID generation (template specialization)
     * @tparam N String length
     * @param str Compile-time constant string
     * @return Compile-time computed UUID object
     *
     * @endif
     */
    template<std::size_t N>
    constexpr auto genUUID(const char (&str)[N]);

    /**
     * @if zh
     * @brief 编译时字符串字面量UUID生成
     * @tparam V 编译时字符串类型
     * @return 编译时计算的UUID对象
     * @note 最高性能的UUID生成方式
     *
     * @else
     * @brief Compile-time string literal UUID generation
     * @tparam V Compile-time string type
     * @return Compile-time computed UUID object
     * @note Highest performance UUID generation method
     *
     * @endif
     */
    template<FStrChar V>
    constexpr auto genUUID();

    /** @concept is_uuid_field
     *
     * @if zh
     * @brief UUID类型概念约束
     * @details 用于模板编程中的UUID类型检查
     *
     * @else
     * @brief UUID type concept constraint
     * @details Used for UUID type checking in template programming
     *
     * @endif
     */
    template<typename T>
    concept is_uuid_field = std::is_same_v<T, UUID>;

}  // namespace minecraft::protocol

#include "mcuuid.hpp"

#endif  // MCUUID_H
