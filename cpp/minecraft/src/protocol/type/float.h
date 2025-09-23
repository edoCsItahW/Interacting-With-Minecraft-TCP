// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file float.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/11 21:40
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef FLOAT_H
#define FLOAT_H
#pragma once

#include <array>
#include <string>

namespace minecraft::protocol {

    /** @struct Float
     *
     * @if zh
     * @brief Minecraft协议单精度浮点数字段封装（4字节IEEE 754大端序）
     * @details
     * - 采用IEEE 754单精度浮点数标准，固定4字节长度
     * - 自动处理主机字节序到网络大端序的转换
     * - 适用于实体旋转角度、比例因子等精度要求适中的场景
     * @note 精度范围：约±3.4e38，精度约6-9位有效数字
     * @warning 单精度浮点数精度有限，不适合高精度计算
     *
     * @else
     * @brief Minecraft protocol single-precision floating point field encapsulation (4-byte IEEE 754 big-endian)
     * @details
     * - Uses IEEE 754 single-precision floating point standard, fixed 4-byte length
     * - Automatically handles host endianness to network big-endian conversion
     * - Suitable for entity rotation angles, scale factors and other moderate precision scenarios
     * @note Precision range: approx. ±3.4e38 with 6-9 significant digits
     * @warning Single precision has limited accuracy, not suitable for high-precision calculations
     *
     * @endif
     * */
    struct Float {
    private:
        /**
         * @if zh
         * @brief 序列化数据缓存（4字节数组）
         * @details 存储大端序表示的浮点数值，优化重复序列化性能
         * @note 使用类型双关避免通过指针直接转换
         *
         * @else
         * @brief Serialized data cache (4-byte array)
         * @details Stores big-endian representation of floating point value, optimizes repeated serialization performance
         * @note Uses type punning to avoid direct pointer conversion
         *
         * @endif
         */
        mutable std::array<std::byte, 4> data{};

        /**
         * @if zh
         * @brief 缓存有效性标志
         * @details 数值修改时自动标记缓存失效
         *
         * @else
         * @brief Cache validity flag
         * @details Automatically marks cache invalid when value is modified
         *
         * @endif
         */
        mutable bool cached = false;

        /**
         * @if zh
         * @brief 序列化字节长度（固定4字节）
         * @invariant 恒为4，符合IEEE 754单精度标准
         *
         * @else
         * @brief Serialized byte length (fixed 4 bytes)
         * @invariant Always 4, conforms to IEEE 754 single-precision standard
         *
         * @endif
         */
        static constexpr std::size_t size_ = 4;

        /**
         * @if zh
         * @brief 单精度浮点数值
         * @details 存储原始的单精度数值，支持特殊浮点值
         *
         * @else
         * @brief Single-precision floating point value
         * @details Stores original float value, supports special floating point values
         *
         * @endif
         */
        float value_ = 0.0f;

    public:
        /**
         * @if zh
         * @brief 原始值类型（float）
         *
         * @else
         * @brief Raw value type (float)
         *
         * @endif
         */
        using type = float;

        /**
         * @if zh
         * @brief 序列化类型（std::array<std::byte, 4>）
         *
         * @else
         * @brief Serialization type (std::array<std::byte, 4>)
         *
         * @endif
         */
        using encodeType = std::array<std::byte, size_>;

        Float() = default;

        /**
         * @if zh
         * @brief 构造函数
         * @param value 单精度浮点数值
         * @post value_ = value, cached = false
         *
         * @else
         * @brief Constructor
         * @param value Single-precision floating point value
         * @post value_ = value, cached = false
         *
         * @endif
         */
        Float(float value);

        /**
         * @if zh
         * @brief 获取序列化长度
         * @return 固定4字节
         * @note 与Double类型形成鲜明对比，体现精度/空间权衡
         *
         * @else
         * @brief Get serialization length
         * @return Fixed 4 bytes
         * @note Contrasts with Double type, demonstrates precision/space trade-off
         *
         * @endif
         */
        [[nodiscard]] static constexpr std::size_t size() noexcept;

        /**
         * @if zh
         * @brief 获取浮点数值
         * @return 当前存储的单精度值
         * @note 轻量级访问，不涉及字节序转换
         *
         * @else
         * @brief Get floating point value
         * @return Currently stored single-precision value
         * @note Lightweight access, no endianness conversion involved
         *
         * @endif
         */
        [[nodiscard]] type value() const noexcept;

        /**
         * @if zh
         * @brief 序列化为字节数组（大端序）
         * @details 使用memcpy安全转换，避免未定义行为
         * @return 4字节大端序表示的浮点数
         * @note 相比Double节省50%存储空间
         *
         * @else
         * @brief Serialize to byte array (big-endian)
         * @details Uses memcpy for safe conversion, avoids undefined behavior
         * @return 4-byte big-endian representation of floating point value
         * @note Saves 50% storage space compared to Double
         *
         * @endif
         */
        [[nodiscard]] encodeType encode() const;

        /**
         * @if zh
         * @brief 从字节数据反序列化
         * @param data 字节数据指针（必须包含至少4字节有效数据）
         * @return 反序列化的单精度浮点数值
         * @pre data != nullptr且指向至少4字节有效数据
         * @note 自动验证数据有效性
         *
         * @else
         * @brief Deserialize from byte data
         * @param data Byte data pointer (must contain at least 4 valid bytes)
         * @return Deserialized single-precision floating point value
         * @pre data != nullptr and points to at least 4 valid bytes
         * @note Automatically validates data integrity
         *
         * @endif
         */
        static auto decode(const std::byte* data);

        /**
         * @if zh
         * @brief 获取可读字符串表示
         * @return 浮点数的字符串表示
         * @note 适合调试和日志输出
         *
         * @else
         * @brief Get human-readable string representation
         * @return String representation of floating point value
         * @note Suitable for debugging and logging
         *
         * @endif
         */
        [[nodiscard]] std::string toString() const;

        /**
         * @if zh
         * @brief 获取十六进制表示
         * @return 4字节十六进制字符串
         * @details 显示原始字节的大端序表示
         *
         * @else
         * @brief Get hexadecimal representation
         * @return 4-byte hexadecimal string
         * @details Displays big-endian representation of raw bytes
         *
         * @endif
         */
        [[nodiscard]] std::string toHexString() const;
    };

    /** @concept is_float_field
     *
     * @if zh
     * @brief 类型特征检查：是否为Float类型
     * @details 用于区分单精度和双精度浮点类型
     *
     * @else
     * @brief Type trait check: whether is Float type
     * @details Used to distinguish between single and double precision floating point types
     *
     * @endif
     */
    template<typename T>
    concept is_float_field = std::is_same_v<T, Float>;

}  // namespace minecraft::protocol

#include "float.hpp"

#endif  // FLOAT_H
