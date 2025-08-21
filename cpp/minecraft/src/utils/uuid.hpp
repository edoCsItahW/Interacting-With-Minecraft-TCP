// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file uuid.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 17:08
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef UUID_HPP
#define UUID_HPP
#pragma once

#include "sha1.h"
#include "utils.h"
#include <sstream>

namespace minecraft {
    namespace detail {
        template<std::size_t N>
        constexpr std::array<uint8_t, 16> UUID::generate(const char (&str)[N]) {
            std::array<uint8_t, N + 16> buffer{};
            std::size_t pos = 0;

            for (uint8_t byte : NAMESPACE_NIL) buffer[pos++] = byte;
            for (char c : str) buffer[pos++] = static_cast<uint8_t>(c);

            auto hash = sha1(buffer);

            std::array<uint8_t, 16> result{};
            for (std::size_t i = 0; i < 16; ++i) result[i] = hash[i];

            result[6] = result[6] & 0x0F | 0x50;  // 版本5
            result[8] = result[8] & 0x3F | 0x80;  // 变体RFC4122

            return result;
        }

        template<std::size_t N>
        constexpr std::array<uint8_t, 16> UUID::generate(const std::array<uint8_t, N>& data) {
            std::array<uint8_t, N + 16> buffer{};
            std::size_t pos = 0;

            for (uint8_t byte : NAMESPACE_NIL) buffer[pos++] = byte;
            for (uint8_t byte : data) buffer[pos++] = byte;

            auto hash = sha1(buffer);

            std::array<uint8_t, 16> result{};
            for (std::size_t i = 0; i < 16; ++i) result[i] = hash[i];

            result[6] = result[6] & 0x0F | 0x50;  // 版本5
            result[8] = result[8] & 0x3F | 0x80;  // 变体RFC4122

            return result;
        }
    }  // namespace detail

    template<std::size_t N>
    constexpr auto uuid(const char (&data)[N]) {
        return detail::UUID::generate(data);
    }

    template<std::size_t N>
    constexpr auto uuid(const std::array<uint8_t, N>& data) {
        return detail::UUID::generate(data);
    }

    constexpr auto uuidToArr(const std::array<uint8_t, 16>& uuid) -> std::array<char, 36> {
        std::array<char, 36> str{};
        constexpr char hex[] = "0123456789abcdef";
        size_t n             = 0;

        for (size_t i = 0; i < 16; ++i) {
            if (i == 4 || i == 6 || i == 8 || i == 10) str[n++] = '-';
            str[n++] = hex[uuid[i] >> 4 & 0x0F];
            str[n++] = hex[uuid[i] & 0x0F];
        }
        return str;
    }

    constexpr auto uuidToStr(const std::array<uint8_t, 16>& uuid) {
        std::stringstream ss;

        ss << std::hex << std::setfill('0');

        for (size_t i = 0; i < 16; ++i) {
            if (i == 4 || i == 6 || i == 8 || i == 10) ss << '-';
            ss << std::setw(2) << static_cast<int>(uuid[i]);
        }

        return ss.str();
    }
}  // namespace minecraft

#endif  // UUID_HPP
