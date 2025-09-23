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
     * @brief Minecraft协议布尔字段封装（1字节表示true/false）
     * @details
     * - 使用单字节表示布尔值：0x00表示false，0x01表示true
     * - 提供布尔值序列化缓存机制
     * @note 线程安全：const方法线程安全，非const方法需外部同步
     *
     * @else
     * @brief Minecraft protocol boolean field encapsulation (1 byte for true/false)
     * @details
     * - Uses single byte to represent boolean: 0x00 for false, 0x01 for true
     * - Provides boolean serialization caching mechanism
     * @note Thread safety: const methods are thread-safe, non-const methods require external synchronization
     *
     * @endif
     * */
    struct Boolean {
    private:
        /**
         * @if zh
         * @brief 序列化字节长度（固定1字节）
         * @invariant 恒为1
         *
         * @else
         * @brief Serialized byte length (fixed 1 byte)
         * @invariant Always 1
         *
         * @endif
         */
        static constexpr std::size_t size_ = 1;

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
        mutable std::array<std::byte, size_> data{};

        /**
         * @if zh
         * @brief 布尔值
         * @invariant 始终为true或false
         *
         * @else
         * @brief Boolean value
         * @invariant Always true or false
         *
         * @endif
         */
        bool value_;

    public:
        /**
         * @if zh
         * @brief 原始值类型（bool）
         *
         * @else
         * @brief Raw value type (bool)
         *
         * @endif
         */
        using type = bool;

        /**
         * @if zh
         * @brief 序列化类型（std::array<std::byte, 1>）
         *
         * @else
         * @brief Serialization type (std::array<std::byte, 1>)
         *
         * @endif
         */
        using encodeType = std::array<std::byte, size_>;

        Boolean() = default;

        /**
         * @if zh
         * @brief 构造函数
         * @param value 布尔值
         * @post value_ = value
         *
         * @else
         * @brief Constructor
         * @param value Boolean value
         * @post value_ = value
         *
         * @endif
         */
        Boolean(bool value);

        /**
         * @if zh
         * @brief 获取序列化长度
         * @return 固定1字节
         *
         * @else
         * @brief Get serialization length
         * @return Fixed 1 byte
         *
         * @endif
         */
        [[nodiscard]] static constexpr std::size_t size();

        /**
         * @if zh
         * @brief 获取布尔值
         * @return true或false
         *
         * @else
         * @brief Get boolean value
         * @return true or false
         *
         * @endif
         */
        [[nodiscard]] type value() const;

        /**
         * @if zh
         * @brief 序列化为字节数组
         * @details 使用缓存机制避免重复计算
         * @return 包含单个字节的数组（0x00或0x01）
         *
         * @else
         * @brief Serialize to byte array
         * @details Uses caching to avoid recomputation
         * @return Single-byte array (0x00 or 0x01)
         *
         * @endif
         */
        [[nodiscard]] encodeType encode() const;

        /**
         * @if zh
         * @brief 从字节数据反序列化
         * @param data 字节数据指针（必须包含至少1字节）
         * @return 反序列化的Boolean对象
         * @pre data != nullptr
         *
         * @else
         * @brief Deserialize from byte data
         * @param data Byte data pointer (must contain at least 1 byte)
         * @return Deserialized Boolean object
         * @pre data != nullptr
         *
         * @endif
         */
        static auto decode(const std::byte* data);

        /**
         * @if zh
         * @brief 获取可读字符串表示
         * @return "true"或"false"
         *
         * @else
         * @brief Get human-readable string representation
         * @return "true" or "false"
         *
         * @endif
         */
        [[nodiscard]] std::string toString() const;

        /**
         * @if zh
         * @brief 获取十六进制表示
         * @return 单字节十六进制字符串（"0x00"或"0x01"）
         *
         * @else
         * @brief Get hexadecimal representation
         * @return Single-byte hex string ("0x00" or "0x01")
         *
         * @endif
         */
        [[nodiscard]] std::string toHexString() const;
    };

    /** @concept is_boolean_field
     *
     * @if zh
     * @brief 类型特征检查：是否为Boolean类型
     * @tparam T 待检查类型
     *
     * @else
     * @brief Type trait check: whether is Boolean type
     * @tparam T Type to be checked
     *
     * @endif
     */
    template<typename T>
    concept is_boolean_field = std::is_same_v<T, Boolean>;

}  // namespace minecraft::protocol

#include "boolean.hpp"

#endif  // BOOLEAN_H
