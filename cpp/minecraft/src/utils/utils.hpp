// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file utils.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 00:13
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef UTILS_HPP
#define UTILS_HPP
#pragma once

#include <iomanip>
#include <sstream>

namespace minecraft {

    namespace detail {
        template<typename T, std::size_t N>
        constexpr auto enumMax() {
            if constexpr (constexpr auto Value = static_cast<T>(N); enumToStr<Value>().find(")") == std::string_view::npos)
                return enumMax<T, N + 1>();

            else
                return N;
        }
    }  // namespace detail

    template<typename T>
    constexpr T binpow(T base, int exp) {
        T result = 1;

        while (exp > 0) {
            if (exp % 2) result *= base;

            base *= base;
            exp >>= 1;
        }

        return result;
    }

    template<typename T>
    std::string toHexString(const T& value) {
        std::ostringstream oss;

        oss << std::hex << std::setfill('0');

        for (const auto byte : value) oss << "\\x" << std::setw(2) << static_cast<int>(byte) << " ";

        return oss.str();
    }

    template<std::size_t N>
    constexpr auto toHexArray(const std::array<uint8_t, N>& digest) {
        std::array<char, 2 * N + 1> hex{};  // 20字节 * 2字符/字节

        constexpr char hexDigits[] = "0123456789abcdef";

        for (size_t i = 0; i < N; ++i) {
            hex[i * 2]     = hexDigits[digest[i] >> 4 & 0x0F];
            hex[i * 2 + 1] = hexDigits[digest[i] & 0x0F];
        }

        hex[2 * N] = '\0';

        return hex;
    }

    template<typename T>
        requires requires(T t) { t.size(); }
    constexpr auto strToBytes(const T& str) {
        std::vector<uint8_t> result{};
        result.resize(str.size());

        for (std::size_t i = 0; i < str.size(); ++i) result[i] = static_cast<uint8_t>(str[i]);

        return result;
    }

    constexpr auto strToBytes(const char* str) { return strToBytes(std::string_view(str)); }

    template<FStr V>
    constexpr auto strToBytes() {
        return strToBytes(V.data);
    }

    constexpr std::size_t strLen(const char* str) { return *str == 0 ? 0 : strLen(str + 1) + 1; }

    template<auto>
    constexpr auto enumToStr() {
        std::string_view name;

#if __GNUC__ || __clang__
        name              = __PRETTY_FUNCTION__;
        std::size_t start = name.find_first_of('=') + 2;
        name              = std::string_view(name.data() + start, name.size() - start - 1);
        start             = name.rfind("::");

#elif _MSC_VER
        name              = __FUNCSIG__;
        std::size_t start = name.find_first_of('<') + 1;
        name              = std::string_view(name.data() + start, name.rfind('>') - start);
        start             = name.rfind("::");

#endif

        return start == std::string_view::npos ? name : std::string_view{name.data() + start + 2, name.size() - start - 2};
    }

    template<typename T>
        requires std::is_enum_v<T>
    constexpr auto enumToStr(T value) {
        constexpr auto num = detail::enumMax<T>();

        constexpr auto names = []<std::size_t... I>(std::index_sequence<I...>) { return std::array<std::string_view, num>{enumToStr<static_cast<T>(I)>()...}; }(std::make_index_sequence<num>{});

        return names[static_cast<std::size_t>(value)];
    }


}  // namespace minecraft


#endif  // UTILS_HPP
