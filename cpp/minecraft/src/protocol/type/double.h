// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file double.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/11 21:55
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef DOUBLE_H
#define DOUBLE_H
#pragma once

#include <array>
#include <string>

namespace minecraft::protocol {

    /** @struct Double
     *
     * @if zh
     * @brief Minecraft协议双精度浮点数字段封装（8字节IEEE 754大端序）
     * @details
     * - 采用IEEE 754双精度浮点数标准，固定8字节长度
     * - 自动处理主机字节序到网络大端序的转换
     * - 支持高精度坐标、速度等需要双精度表示的场景
     * @note 精度范围：约±1.7e308，精度约15-17位有效数字
     * @warning 涉及浮点运算时注意精度损失问题
     *
     * @else
     * @brief Minecraft protocol double-precision floating point field encapsulation (8-byte IEEE 754 big-endian)
     * @details
     * - Uses IEEE 754 double-precision floating point standard, fixed 8-byte length
     * - Automatically handles host endianness to network big-endian conversion
     * - Supports high-precision coordinates, velocity and other scenarios requiring double precision
     * @note Precision range: approx. ±1.7e308 with 15-17 significant digits
     * @warning Be aware of precision loss in floating-point operations
     *
     * @endif
     * */
    struct Double {
    private:
        /**
         * @if zh
         * @brief 序列化数据缓存（8字节数组）
         * @details 存储大端序表示的浮点数值，避免重复转换
         * @note 使用memcpy避免严格别名规则问题
         *
         * @else
         * @brief Serialized data cache (8-byte array)
         * @details Stores big-endian representation of floating point value, avoids repeated conversion
         * @note Uses memcpy to avoid strict aliasing rule issues
         *
         * @endif
         */
        mutable std::array<std::byte, 8> data{};

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
         * @brief 序列化字节长度（固定8字节）
         * @invariant 恒为8，符合IEEE 754双精度标准
         *
         * @else
         * @brief Serialized byte length (fixed 8 bytes)
         * @invariant Always 8, conforms to IEEE 754 double-precision standard
         *
         * @endif
         */
        static constexpr std::size_t size_ = 8;

        /**
         * @if zh
         * @brief 双精度浮点数值
         * @details 存储原始的双精度数值，支持特殊值（NaN、Infinity等）
         *
         * @else
         * @brief Double-precision floating point value
         * @details Stores original double value, supports special values (NaN, Infinity, etc.)
         *
         * @endif
         */
        double value_ = 0.0;

    public:
        /**
         * @if zh
         * @brief 原始值类型（double）
         *
         * @else
         * @brief Raw value type (double)
         *
         * @endif
         */
        using type = double;

        /**
         * @if zh
         * @brief 序列化类型（std::array<std::byte, 8>）
         *
         * @else
         * @brief Serialization type (std::array<std::byte, 8>)
         *
         * @endif
         */
        using encodeType = std::array<std::byte, size_>;

        Double() = default;

        /**
         * @if zh
         * @brief 构造函数
         * @param value 双精度浮点数值
         * @post value_ = value, cached = false
         *
         * @else
         * @brief Constructor
         * @param value Double-precision floating point value
         * @post value_ = value, cached = false
         *
         * @endif
         */
        Double(double value);

        /**
         * @if zh
         * @brief 获取序列化长度
         * @return 固定8字节
         * @note 符合Minecraft协议规范
         *
         * @else
         * @brief Get serialization length
         * @return Fixed 8 bytes
         * @note Complies with Minecraft protocol specification
         *
         * @endif
         */
        [[nodiscard]] static constexpr std::size_t size() noexcept;

        /**
         * @if zh
         * @brief 获取浮点数值
         * @return 当前存储的双精度值
         * @note 不触发序列化操作，性能高效
         *
         * @else
         * @brief Get floating point value
         * @return Currently stored double value
         * @note Does not trigger serialization, high performance
         *
         * @endif
         */
        [[nodiscard]] type value() const noexcept;

        /**
         * @if zh
         * @brief 序列化为字节数组（大端序）
         * @details 自动处理字节序转换，支持缓存优化
         * @return 8字节大端序表示的浮点数
         * @throws 不会抛出异常
         *
         * @else
         * @brief Serialize to byte array (big-endian)
         * @details Automatically handles endianness conversion, supports caching optimization
         * @return 8-byte big-endian representation of floating point value
         * @throws No exceptions thrown
         *
         * @endif
         */
        [[nodiscard]] encodeType encode() const;

        /**
         * @if zh
         * @brief 从字节数据反序列化
         * @param data 字节数据指针（必须包含至少8字节有效数据）
         * @return 反序列化的双精度浮点数值
         * @pre data != nullptr且指向至少8字节有效数据
         * @note 自动处理大端序到主机字节序的转换
         *
         * @else
         * @brief Deserialize from byte data
         * @param data Byte data pointer (must contain at least 8 valid bytes)
         * @return Deserialized double-precision floating point value
         * @pre data != nullptr and points to at least 8 valid bytes
         * @note Automatically handles big-endian to host endianness conversion
         *
         * @endif
         */
        static auto decode(const std::byte* data);

        /**
         * @if zh
         * @brief 获取可读字符串表示
         * @return 浮点数的字符串表示（使用std::to_string）
         * @note 可能包含科学计数法表示
         *
         * @else
         * @brief Get human-readable string representation
         * @return String representation of floating point value (using std::to_string)
         * @note May contain scientific notation
         *
         * @endif
         */
        [[nodiscard]] std::string toString() const;

        /**
         * @if zh
         * @brief 获取十六进制表示
         * @return 8字节十六进制字符串
         * @details 显示原始字节的大端序表示
         *
         * @else
         * @brief Get hexadecimal representation
         * @return 8-byte hexadecimal string
         * @details Displays big-endian representation of raw bytes
         *
         * @endif
         */
        [[nodiscard]] std::string toHexString() const;
    };

    /** @concept is_double_field
     *
     * @if zh
     * @brief 类型特征检查：是否为Double类型
     * @details 用于模板特化和概念约束
     *
     * @else
     * @brief Type trait check: whether is Double type
     * @details Used for template specialization and concept constraints
     *
     * @endif
     */
    template<typename T>
    concept is_double_field = std::is_same_v<T, Double>;

}  // namespace minecraft::protocol

#include "double.hpp"

#endif  // DOUBLE_H