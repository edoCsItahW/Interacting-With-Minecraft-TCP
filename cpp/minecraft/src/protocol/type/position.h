// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file position.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/13 17:06
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef POSITION_H
#define POSITION_H
#pragma once

#include <array>
#include <string>
#include <tuple>

namespace minecraft::protocol {

    /** @struct Position
     *
     * @if zh
     * @brief Minecraft协议三维坐标位置封装（8字节压缩编码）
     * @details
     * - 使用64位整数高效编码三维坐标（x26位，y12位，z26位）
     * - 支持超大世界坐标范围（±33,554,432方块）
     * - 自动处理坐标值的符号扩展和边界检查
     * @note 采用Minecraft特有的位压缩算法，优化网络传输
     * @warning 坐标值超出范围时自动进行模运算处理
     *
     * @else
     * @brief Minecraft protocol 3D coordinate position encapsulation (8-byte compressed encoding)
     * @details
     * - Efficiently encodes 3D coordinates using 64-bit integer (x26-bit, y12-bit, z26-bit)
     * - Supports超大 world coordinate range (±33,554,432 blocks)
     * - Automatically handles coordinate sign extension and boundary checking
     * @note Uses Minecraft-specific bit compression algorithm, optimizes network transmission
     * @warning Automatically performs modulo operation when coordinate values exceed range
     *
     * @endif
     */
    struct Position {
    private:
        /**
         * @if zh
         * @brief 序列化字节长度（固定8字节）
         * @invariant 恒为8，使用64位整数编码
         *
         * @else
         * @brief Serialized byte length (fixed 8 bytes)
         * @invariant Always 8, uses 64-bit integer encoding
         *
         * @endif
         */
        static constexpr std::size_t size_ = 8;

        /**
         * @if zh
         * @brief 坐标值元组存储
         * @details 使用tuple存储(x, y, z)三维坐标
         *
         * @else
         * @brief Coordinate value tuple storage
         * @details Uses tuple to store (x, y, z) 3D coordinates
         *
         * @endif
         */
        std::tuple<int64_t, int64_t, int64_t> value_;

        /**
         * @if zh
         * @brief 缓存有效性标志
         * @details 坐标变化时自动失效
         *
         * @else
         * @brief Cache validity flag
         * @details Automatically invalidated when coordinates change
         *
         * @endif
         */
        mutable bool cached = false;

        /**
         * @if zh
         * @brief 序列化数据缓存
         * @details 存储压缩后的64位坐标表示
         *
         * @else
         * @brief Serialized data cache
         * @details Stores compressed 64-bit coordinate representation
         *
         * @endif
         */
        mutable std::array<std::byte, size_> data{};

        /**
         * @if zh
         * @brief X坐标值
         * @details 26位有符号整数，范围±33,554,432
         *
         * @else
         * @brief X coordinate value
         * @details 26-bit signed integer, range ±33,554,432
         *
         * @endif
         */
        int64_t x_;

        /**
         * @if zh
         * @brief Y坐标值
         * @details 12位有符号整数，范围±2,048
         *
         * @else
         * @brief Y coordinate value
         * @details 12-bit signed integer, range ±2,048
         *
         * @endif
         */
        int64_t y_;

        /**
         * @if zh
         * @brief Z坐标值
         * @details 26位有符号整数，范围±33,554,432
         *
         * @else
         * @brief Z coordinate value
         * @details 26-bit signed integer, range ±33,554,432
         *
         * @endif
         */
        int64_t z_;

    public:
        /**
         * @if zh
         * @brief 原始值类型（std::tuple<int64_t, int64_t, int64_t>）
         *
         * @else
         * @brief Raw value type (std::tuple<int64_t, int64_t, int64_t>)
         *
         * @endif
         */
        using type = std::tuple<int64_t, int64_t, int64_t>;

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

        /**
         * @if zh
         * @brief 默认构造函数
         * @post 创建原点坐标(0, 0, 0)
         *
         * @else
         * @brief Default constructor
         * @post Creates origin coordinate (0, 0, 0)
         *
         * @endif
         */
        Position();

        /**
         * @if zh
         * @brief 坐标值构造函数
         * @param x X坐标值
         * @param y Y坐标值
         * @param z Z坐标值
         * @post 自动处理坐标范围验证和压缩
         *
         * @else
         * @brief Coordinate value constructor
         * @param x X coordinate value
         * @param y Y coordinate value
         * @param z Z coordinate value
         * @post Automatically handles coordinate range validation and compression
         *
         * @endif
         */
        Position(int64_t x, int64_t y, int64_t z);

        /**
         * @if zh
         * @brief 元组构造函数
         * @param value 包含(x, y, z)的元组
         *
         * @else
         * @brief Tuple constructor
         * @param value Tuple containing (x, y, z)
         *
         * @endif
         */
        Position(const type& value);

        /**
         * @if zh
         * @brief 获取序列化长度
         * @return 固定8字节
         * @note 使用高效的位压缩编码
         *
         * @else
         * @brief Get serialization length
         * @return Fixed 8 bytes
         * @note Uses efficient bit compression encoding
         *
         * @endif
         */
        [[nodiscard]] static std::size_t size();

        /**
         * @if zh
         * @brief 获取坐标值
         * @return 包含(x, y, z)的元组
         *
         * @else
         * @brief Get coordinate values
         * @return Tuple containing (x, y, z)
         *
         * @endif
         */
        [[nodiscard]] type value() const;

        /**
         * @if zh
         * @brief 序列化为字节数组（位压缩）
         * @details 将三维坐标压缩为64位整数：x(26位) | y(12位) | z(26位)
         * @return 8字节压缩坐标数据
         *
         * @else
         * @brief Serialize to byte array (bit compression)
         * @details Compresses 3D coordinates to 64-bit integer: x(26-bit) | y(12-bit) | z(26-bit)
         * @return 8-byte compressed coordinate data
         *
         * @endif
         */
        [[nodiscard]] encodeType encode() const;

        /**
         * @if zh
         * @brief 从字节数据反序列化
         * @param data 字节数据指针（必须包含至少8字节）
         * @return 反序列化的Position对象
         * @pre data != nullptr且指向至少8字节有效数据
         * @note 自动处理位解压缩和符号扩展
         *
         * @else
         * @brief Deserialize from byte data
         * @param data Byte data pointer (must contain at least 8 bytes)
         * @return Deserialized Position object
         * @pre data != nullptr and points to at least 8 valid bytes
         * @note Automatically handles bit decompression and sign extension
         *
         * @endif
         */
        static auto decode(const std::byte* data);

        /**
         * @if zh
         * @brief 获取可读字符串表示
         * @return 格式："(x, y, z)"
         *
         * @else
         * @brief Get human-readable string representation
         * @return Format: "(x, y, z)"
         *
         * @endif
         */
        [[nodiscard]] std::string toString() const;

        /**
         * @if zh
         * @brief 获取十六进制表示
         * @return 8字节十六进制字符串
         * @details 显示压缩后的64位坐标值
         *
         * @else
         * @brief Get hexadecimal representation
         * @return 8-byte hexadecimal string
         * @details Displays compressed 64-bit coordinate value
         *
         * @endif
         */
        [[nodiscard]] std::string toHexString() const;
    };

    /** @concept is_position_field
     *
     * @if zh
     * @brief 位置类型概念约束
     * @details 用于模板编程中的位置类型检查
     *
     * @else
     * @brief Position type concept constraint
     * @details Used for position type checking in template programming
     *
     * @endif
     */
    template<typename T>
    concept is_position_field = std::is_same_v<T, Position>;

}  // namespace minecraft::protocol

#include "position.hpp"

#endif  // POSITION_H
