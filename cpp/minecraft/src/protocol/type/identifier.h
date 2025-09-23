// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file identifier.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/14 11:18
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef IDENTIFIER_H
#define IDENTIFIER_H
#pragma once

#include "str.h"

namespace minecraft::protocol {

    /** @struct Identifier
     *
     * @if zh
     * @brief Minecraft协议标识符字段封装（命名空间:路径格式）
     * @details
     * - 继承自String类型，专门用于Minecraft资源定位符
     * - 遵循"namespace:path"格式（如"minecraft:stone"）
     * - 自动验证标识符格式有效性
     * @note 默认命名空间为"minecraft"，可省略前缀
     * @see Minecraft官方标识符规范
     *
     * @else
     * @brief Minecraft protocol identifier field encapsulation (namespace:path format)
     * @details
     * - Inherits from String type, specifically for Minecraft resource locators
     * - Follows "namespace:path" format (e.g., "minecraft:stone")
     * - Automatically validates identifier format validity
     * @note Default namespace is "minecraft", prefix can be omitted
     * @see Minecraft official identifier specification
     *
     * @endif
     * */
    struct Identifier : String {
        /**
         * @if zh
         * @brief 原始值类型（std::string）
         * @details 存储完整的标识符字符串
         *
         * @else
         * @brief Raw value type (std::string)
         * @details Stores complete identifier string
         *
         * @endif
         */
        using type = std::string;

        /**
         * @if zh
         * @brief 序列化类型（std::vector<std::byte>）
         * @details 使用变长编码，继承String的序列化机制
         *
         * @else
         * @brief Serialization type (std::vector<std::byte>)
         * @details Uses variable-length encoding, inherits String's serialization mechanism
         *
         * @endif
         */
        using serializeType = std::vector<std::byte>;

        /**
         * @if zh
         * @brief 默认构造函数
         * @post 创建空标识符
         *
         * @else
         * @brief Default constructor
         * @post Creates empty identifier
         *
         * @endif
         */
        Identifier();

        /**
         * @if zh
         * @brief 字符串构造函数
         * @param str 标识符字符串（自动验证格式）
         * @throws 可能抛出格式验证异常
         * @note 支持省略命名空间的简写形式
         *
         * @else
         * @brief String constructor
         * @param str Identifier string (automatically validates format)
         * @throws May throw format validation exceptions
         * @note Supports shorthand form without namespace
         *
         * @endif
         */
        Identifier(const std::string& str);

        /**
         * @if zh
         * @brief 从字节数据反序列化
         * @param data 字节数据指针
         * @return 反序列化的Identifier对象
         * @details 重用String的反序列化逻辑，确保编码一致性
         *
         * @else
         * @brief Deserialize from byte data
         * @param data Byte data pointer
         * @return Deserialized Identifier object
         * @details Reuses String's deserialization logic, ensures encoding consistency
         *
         * @endif
         */
        static auto decode(const std::byte* data);

        // 注：其他方法（encode, toString等）继承自String类
    };

    /** @concept is_identifier_field
     *
     * @if zh
     * @brief 类型特征检查：是否为Identifier类型
     * @details 专门用于标识符相关的模板特化
     *
     * @else
     * @brief Type trait check: whether is Identifier type
     * @details Specifically for identifier-related template specialization
     *
     * @endif
     */
    template<typename T>
    concept is_identifier_field = std::is_same_v<T, Identifier>;

}  // namespace minecraft::protocol

#include "identifier.hpp"

#endif  // IDENTIFIER_H
