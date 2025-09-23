// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file str.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/17 15:00
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef STR_H
#define STR_H
#pragma once

#include <string>
#include <vector>

namespace minecraft::protocol {

    /**
     * @if zh
     * @brief VarInt解析辅助函数
     * @tparam T 整数类型（int或long）
     * @param data 字节数据指针
     * @return 解析后的整数值和读取的字节数
     * @throws 当VarInt过长时抛出std::runtime_error
     *
     * @else
     * @brief VarInt parsing helper function
     * @tparam T Integer type (int or long)
     * @param data Byte data pointer
     * @return Parsed integer value and number of bytes read
     * @throws std::runtime_error when VarInt is too long
     *
     * @endif
     */
    template<detail::intOrLong T>
    static std::pair<T, int> parseVarInt(const std::byte *data);

    /** @struct String
     *
     * @if zh
     * @brief Minecraft协议字符串字段封装（VarInt长度前缀 + UTF-8数据）
     * @details
     * - 使用VarInt编码字符串长度，优化短字符串的存储效率
     * - 支持UTF-8编码的文本数据，自动处理长度计算
     * - 提供安全的字符串操作和序列化缓存机制
     * @note 最大长度受VarInt范围限制（约2^31-1字符）
     * @warning 非UTF-8字符串可能导致编码问题
     *
     * @else
     * @brief Minecraft protocol string field encapsulation (VarInt length prefix + UTF-8 data)
     * @details
     * - Uses VarInt encoding for string length, optimizes storage efficiency for short strings
     * - Supports UTF-8 encoded text data, automatically handles length calculation
     * - Provides safe string operations and serialization caching mechanism
     * @note Maximum length limited by VarInt range (approx. 2^31-1 characters)
     * @warning Non-UTF-8 strings may cause encoding issues
     *
     * @endif
     */
    struct String {
    protected:
        /**
         * @if zh
         * @brief 序列化数据缓存
         * @details 存储VarInt长度前缀和字符串数据的拼接
         *
         * @else
         * @brief Serialized data cache
         * @details Stores concatenation of VarInt length prefix and string data
         *
         * @endif
         */
        mutable std::vector<std::byte> data;

        /**
         * @if zh
         * @brief 缓存有效性标志
         * @details 字符串内容变化时自动失效
         *
         * @else
         * @brief Cache validity flag
         * @details Automatically invalidated when string content changes
         *
         * @endif
         */
        mutable bool cached = false;

        /**
         * @if zh
         * @brief 序列化总长度
         * @details 动态计算：VarInt长度 + 字符串UTF-8字节长度
         *
         * @else
         * @brief Total serialization length
         * @details Dynamically calculated: VarInt length + string UTF-8 byte length
         *
         * @endif
         */
        std::size_t size_{0};

        /**
         * @if zh
         * @brief 字符串值
         * @details 存储原始的UTF-8字符串数据
         *
         * @else
         * @brief String value
         * @details Stores original UTF-8 string data
         *
         * @endif
         */
        std::string value_;

    public:
        /**
         * @if zh
         * @brief 原始值类型（std::string）
         *
         * @else
         * @brief Raw value type (std::string)
         *
         * @endif
         */
        using type = std::string;

        /**
         * @if zh
         * @brief 序列化类型（std::vector<std::byte>）
         * @details 变长字节向量，包含长度前缀和字符串数据
         *
         * @else
         * @brief Serialization type (std::vector<std::byte>)
         * @details Variable-length byte vector containing length prefix and string data
         *
         * @endif
         */
        using encodeType = std::vector<std::byte>;

        String() = default;

        /**
         * @if zh
         * @brief 字符串构造函数
         * @param value UTF-8字符串值
         * @post 自动计算序列化总长度
         *
         * @else
         * @brief String constructor
         * @param value UTF-8 string value
         * @post Automatically calculates total serialization length
         *
         * @endif
         */
        String(const std::string &value);

        String(const String &other) = default;

        String(String &&other) = default;

        String &operator=(const String &other) = default;

        String &operator=(String &&other) = default;

        /**
         * @if zh
         * @brief 获取序列化长度
         * @return VarInt长度前缀 + 字符串UTF-8字节长度
         *
         * @else
         * @brief Get serialization length
         * @return VarInt length prefix + string UTF-8 byte length
         *
         * @endif
         */
        [[nodiscard]] std::size_t size() const;

        /**
         * @if zh
         * @brief 获取字符串值
         * @return 原始UTF-8字符串
         *
         * @else
         * @brief Get string value
         * @return Original UTF-8 string
         *
         * @endif
         */
        [[nodiscard]] type value() const;

        /**
         * @if zh
         * @brief 序列化为字节数组
         * @details 先序列化VarInt长度前缀，再序列化UTF-8字符串数据
         * @return 包含完整序列化数据的字节向量
         *
         * @else
         * @brief Serialize to byte array
         * @details First serializes VarInt length prefix, then serializes UTF-8 string data
         * @return Byte vector containing complete serialized data
         *
         * @endif
         */
        [[nodiscard]] encodeType encode() const;

        /**
         * @if zh
         * @brief 从字节数据反序列化
         * @param data 字节数据指针
         * @return 反序列化的String对象
         * @pre data必须包含有效的VarInt长度前缀和相应长度的字符串数据
         *
         * @else
         * @brief Deserialize from byte data
         * @param data Byte data pointer
         * @return Deserialized String object
         * @pre data must contain valid VarInt length prefix and corresponding length string data
         *
         * @endif
         */
        static auto decode(const std::byte *data);

        /**
         * @if zh
         * @brief 获取可读字符串表示
         * @return 安全格式化的字符串，非打印字符显示为十六进制转义
         *
         * @else
         * @brief Get human-readable string representation
         * @return Safely formatted string, non-printable characters displayed as hex escapes
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

    /** @concept is_string_field
     *
     * @if zh
     * @brief 字符串类型概念约束
     * @details 用于模板编程中的字符串类型检查
     *
     * @else
     * @brief String type concept constraint
     * @details Used for string type checking in template programming
     *
     * @endif
     */
    template<typename T>
    concept is_string_field = std::is_same_v<T, String>;

}  // namespace minecraft::protocol

#include "str.hpp"

#endif  // STR_H
