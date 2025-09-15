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

    namespace detail {
        template<typename T>
        constexpr auto javaUUID(T str);

        std::string uuidToString(const std::array<std::byte, 16>& uuidBytes);
    }

    struct UUID {
    private:

        static constexpr std::size_t size_ = 16;

        std::array<std::byte, size_> value_;

    public:

        using type = std::array<std::byte, size_>;

        using serializeType = std::array<std::byte, size_>;

        UUID();

        UUID(const std::array<std::byte, size_>& value);

        UUID(const UUID&) = default;

        UUID(UUID&&) = default;

        UUID& operator=(const UUID&) = default;

        UUID& operator=(UUID&&) = default;

        [[nodiscard]] static constexpr std::size_t size();

        [[nodiscard]] type value() const;

        [[nodiscard]] serializeType serialize() const;

        static auto deserialize(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<typename T>
    auto genUUID(T str);

    template<std::size_t N>
    constexpr auto genUUID(const char (&str)[N]);

    template<FStrChar V>
    constexpr auto genUUID();

    template<typename T>
    concept is_uuid_field = std::is_same_v<T, UUID>;

}  // namespace minecraft::protocol

#include "mcuuid.hpp"

#endif  // MCUUID_H
