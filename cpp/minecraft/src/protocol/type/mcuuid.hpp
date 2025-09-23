// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file mcuuid.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 19:04
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef MCUUID_HPP
#define MCUUID_HPP
#pragma once

#include "../../utils/md5.h"
#include "../../utils/utils.h"
#include "../../utils/uuid.h"
#include <sstream>

namespace minecraft::protocol {
    namespace detail {
        template<typename T>
        constexpr auto javaUUID(T str) {
            auto hash = md5(str);

            hash[6] = hash[6] & 0x0f | 0x30;
            hash[8] = hash[8] & 0x3f | 0x80;

            auto u = uuid(hash);

            std::array<std::byte, 16> result{};

            for (std::size_t i{0}; i < 16; i++) result[i] = static_cast<std::byte>(u[i]);

            return result;
        }

        inline std::string uuidToString(const std::array<std::byte, 16>& uuidBytes) {
            std::stringstream ss;

            for (std::size_t i{0}; i < 16; i++) {
                ss << std::hex << static_cast<int>(uuidBytes[i]);

                if (i == 3 || i == 5 || i == 7 || i == 9) ss << "-";
            }

            return ss.str();
        }
    }  // namespace detail

    inline UUID::UUID()
        : value_{} {}

    inline UUID::UUID(const std::array<std::byte, 16>& value)
        : value_(value) {}

    constexpr std::size_t UUID::size() { return size_; }

    inline UUID::type UUID::value() const { return value_; }

    inline UUID::encodeType UUID::encode() const { return value_; }

    inline auto UUID::decode(const std::byte* data) {
        std::array<std::byte, 16> result{};

        for (std::size_t i{0}; i < 16; i++) result[i] = data[i];

        return UUID(result);
    }

    inline std::string UUID::toString() const { return detail::uuidToString(value_); }

    inline std::string UUID::toHexString() const { return minecraft::toHexString(value_); }

    template<typename T>
    auto genUUID(T str) {
        return detail::javaUUID(str);
    }

    template<std::size_t N>
    constexpr auto genUUID(const char (&str)[N]) {
        return detail::javaUUID(str);
    }

    template<FStrChar V>
    constexpr auto genUUID() {
        return detail::javaUUID(V.data);
    }

}  // namespace minecraft::protocol

#endif  // MCUUID_HPP
