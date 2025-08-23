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

    template<auto...>
    struct UUID;

    template<>
    struct UUID<> {
        using type = std::array<std::byte, 16>;

        static constexpr std::size_t size = 16;

        std::array<std::byte, size> value;

        UUID();

        UUID(const std::array<std::byte, size>& value);

        UUID(const std::string& str);

        UUID(const UUID&) = default;

        UUID(UUID&&) = default;

        UUID& operator=(const UUID&) = default;

        UUID& operator=(UUID&&) = default;

        [[nodiscard]] auto serialize() const;

        static auto deserialize(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    UUID(const std::array<std::byte, 16>&) -> UUID<>;

    UUID(const std::string&) -> UUID<>;

    template<std::array<std::byte, 16> V>
    struct UUID<V> {
        using type = std::array<std::byte, 16>;

        static constexpr std::size_t size = 16;

        static constexpr auto value = V;

        UUID() = default;

        UUID(const UUID&) = default;

        UUID(UUID&&) = default;

        UUID& operator=(const UUID&) = default;

        UUID& operator=(UUID&&) = default;

        static constexpr auto serialize();

        static auto deserialize(const std::byte* data);

        static std::string toString();

        static std::string toHexString();
    };

    template<FStrChar V>
    struct UUID<V> {
        using type = std::string;

        static constexpr std::size_t size = 16;

        static constexpr std::array<char, V.size> value = V.data;

        static constexpr auto serialize();

        static auto deserialize(const std::byte* data);

        static std::string toString();

        static std::string toHexString();
    };

    template<typename T>
    auto genUUID(T str);

    template<std::size_t N>
    constexpr auto genUUID(const char (&str)[N]);

    template<FStrChar V>
    constexpr auto genUUID();

    using UUIDType = UUID<>;

    template<typename>
    struct is_uuid_field : std::false_type {};

    template<>
    struct is_uuid_field<UUID<>> : std::true_type {};

    template<std::array<std::byte, 16> V>
    struct is_uuid_field<UUID<V>> : std::true_type {};

    template<FStrChar V>
    struct is_uuid_field<UUID<V>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_uuid_field_v = is_uuid_field<T>::value;

    template<typename T>
    concept uuidField = is_uuid_field_v<T>;

}  // namespace minecraft::protocol

#include "mcuuid.hpp"

#endif  // MCUUID_H
