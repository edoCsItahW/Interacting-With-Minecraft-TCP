// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file angle.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/11 22:07
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef ANGLE_H
#define ANGLE_H
#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace minecraft::protocol {

    /** @struct Angle
     *
     * @if zh
     * @brief Minecraft协议角度字段封装（1字节表示0-360度）
     * @details
     * - 使用单字节(0-255)表示360度范围，精度约1.406度/步
     * - 自动处理角度归一化（支持负值和超360度输入）
     * - 提供度/弧度转换和序列化缓存机制
     * @note 线程安全：const方法线程安全，非const方法需外部同步
     *
     *
     * @else
     * @brief Minecraft protocol angle field encapsulation (1 byte for 0-360 degrees)
     * @details
     * - Uses single byte (0-255) to represent 360 degrees with ~1.406 degrees/step precision
     * - Automatic angle normalization (supports negative and >360 degree inputs)
     * - Provides degree/radian conversion and serialization caching
     * @note Thread safety: const methods are thread-safe, non-const methods require external synchronization
     *
     * @endif
     * */
    struct Angle {
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
         * @brief 单步角度值（360/256度）
         * @details 用于字节→角度转换：degrees = value_ * DEGREES_PER_STEP
         *
         * @else
         * @brief Angle value per step (360/256 degrees)
         * @details For byte→degree conversion: degrees = value_ * DEGREES_PER_STEP
         *
         * @endif
         */
        static constexpr float DEGREES_PER_STEP = 360.0f / 256.0f;

        /**
         * @if zh
         * @brief 单度步长值（256/360步）
         * @details 用于角度→字节转换：value_ = round(degrees * STEPS_PER_DEGREE) % 256
         *
         * @else
         * @brief Steps per degree (256/360 steps)
         * @details For degree→byte conversion: value_ = round(degrees * STEPS_PER_DEGREE) % 256
         *
         * @endif
         */
        static constexpr float STEPS_PER_DEGREE = 256.0f / 360.0f;

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
         * @brief 角度字节值（0-255）
         * @invariant 始终满足 0 ≤ value_ ≤ 255
         *
         * @else
         * @brief Angle byte value (0-255)
         * @invariant Always satisfies 0 ≤ value_ ≤ 255
         *
         * @endif
         */
        uint8_t value_ = 0;

        /**
         * @if zh
         * @brief 角度归一化处理
         * @tparam T 输入类型（支持整型/浮点型）
         * @param degrees 输入角度值（可超出0-360范围）
         * @return 归一化后的字节值（0-255）
         * @details 处理流程：1.取模360 → 2.处理负数 → 3.四舍五入 → 4.取模256
         *
         * @else
         * @brief Angle normalization
         * @tparam T Input type (integer or floating point)
         * @param degrees Input angle (can exceed 0-360 range)
         * @return Normalized byte value (0-255)
         * @details Process: 1.Modulo 360 → 2.Handle negative → 3.Round → 4.Modulo 256
         *
         * @endif
         */
        template<typename T>
        uint8_t normalize(T degrees);

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

    public:
        /**
         * @if zh
         * @brief 原始值类型（uint8_t）
         *
         * @else
         * @brief Raw value type (uint8_t)
         *
         * @endif
         */
        using type = uint8_t;

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

        Angle() = default;

        /**
         * @if zh
         * @brief 构造函数（自动归一化输入角度）
         * @tparam T 输入类型（整型/浮点型）
         * @param value 输入角度值
         * @post value_ = normalize(value)
         *
         * @else
         * @brief Constructor (automatically normalizes input angle)
         * @tparam T Input type (integer or floating point)
         * @param value Input angle value
         * @post value_ = normalize(value)
         *
         * @endif
         */
        template<typename T>
        Angle(T value);

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
         * @brief 获取角度字节值
         * @return 0-255范围内的字节值
         *
         * @else
         * @brief Get angle byte value
         * @return Byte value in range 0-255
         *
         * @endif
         */
        [[nodiscard]] type value() const;

        /**
         * @if zh
         * @brief 序列化为字节数组
         * @details 使用缓存机制避免重复计算
         * @return 包含单个字节的数组
         * @post cached = true（如果之前为false）
         *
         * @else
         * @brief Serialize to byte array
         * @details Uses caching to avoid recomputation
         * @return Single-byte array
         * @post cached = true (if previously false)
         *
         * @endif
         */
        [[nodiscard]] encodeType encode() const;

        /**
         * @if zh
         * @brief 从字节数据反序列化
         * @param data 字节数据指针（必须包含至少1字节）
         * @return 反序列化的Angle对象
         * @pre data != nullptr
         *
         * @else
         * @brief Deserialize from byte data
         * @param data Byte data pointer (must contain at least 1 byte)
         * @return Deserialized Angle object
         * @pre data != nullptr
         *
         * @endif
         */
        static auto decode(const std::byte* data);

        /**
         * @if zh
         * @brief 获取可读字符串表示
         * @return 格式："角度° (步长 steps)"（示例："90° (64 steps)"）
         *
         * @else
         * @brief Get human-readable string representation
         * @return Format: "degrees° (steps steps)" (e.g., "90° (64 steps)")
         *
         * @endif
         */
        [[nodiscard]] std::string toString() const;

        /**
         * @if zh
         * @brief 获取十六进制表示
         * @return 单字节十六进制字符串（示例："0x40"）
         *
         * @else
         * @brief Get hexadecimal representation
         * @return Single-byte hex string (e.g., "0x40")
         *
         * @endif
         */
        [[nodiscard]] std::string toHexString() const;

        /**
         * @if zh
         * @brief 转换为浮点角度值
         * @return 0.0-360.0范围内的角度值
         *
         * @else
         * @brief Convert to floating-point degrees
         * @return Degree value in range 0.0-360.0
         *
         * @endif
         */
        float toDegrees() const;

        /**
         * @if zh
         * @brief 转换为弧度值
         * @return 0-2π范围内的弧度值
         *
         * @else
         * @brief Convert to radians
         * @return Radian value in range 0-2π
         *
         * @endif
         */
        float toRadians() const;
    };

    /** @concept is_angle_field
     *
     * @if zh
     * @brief 类型特征检查：是否为Angle类型
     *
     * @else
     * @brief Type trait check: whether is Angle type
     *
     * @endif
     */
    template<typename T>
    concept is_angle_field = std::is_same_v<T, Angle>;

}  // namespace minecraft::protocol

#include "angle.hpp"

#endif  // ANGLE_H
