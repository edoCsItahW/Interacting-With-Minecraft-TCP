// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file md5.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 17:42
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef MD5_H
#define MD5_H
#pragma once

#include <array>

namespace minecraft {

    namespace detail {

        struct MD5 {
        private:
            using Func = uint32_t (*)(uint32_t, uint32_t, uint32_t);

            static constexpr uint32_t CBLOCK = 64;
            static constexpr uint32_t LBLOCK = CBLOCK / 4;

            static constexpr auto makeDigest(const std::array<uint32_t, 4>& state) noexcept;

            static constexpr uint32_t calcF(uint32_t b, uint32_t c, uint32_t d) noexcept;

            static constexpr uint32_t calcG(uint32_t b, uint32_t c, uint32_t d) noexcept;

            static constexpr uint32_t calcH(uint32_t b, uint32_t c, uint32_t d) noexcept;

            static constexpr uint32_t calcI(uint32_t b, uint32_t c, uint32_t d) noexcept;

            static constexpr Func Funcs[4]{calcF, calcG, calcH, calcI};

            static constexpr uint32_t rotate(uint32_t x, uint32_t n) noexcept;

            static constexpr uint32_t calcT(Func f, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) noexcept;

            static constexpr uint32_t toUint32(const unsigned char* data) noexcept;

            static constexpr std::array<uint32_t, CBLOCK> G{0, 1, 2,  3,  4, 5, 6, 7,  8,  9, 10, 11, 12, 13, 14, 15, 1, 6, 11, 0, 5,  10, 15, 4, 9, 14, 3, 8,  13, 2,  7, 12,
                                                            5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3,  6,  9,  12, 15, 2,  0, 7, 14, 5, 12, 3,  10, 1, 8, 15, 6, 13, 4,  11, 2, 9};

            static constexpr std::array<uint32_t, CBLOCK> K{0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1,
                                                            0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453,
                                                            0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942,
                                                            0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                                                            0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d,
                                                            0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

            static constexpr std::array<uint32_t, LBLOCK> S{7, 12, 17, 22, 5, 9, 14, 20, 4, 11, 16, 23, 6, 10, 15, 21};

            static constexpr std::array<char, CBLOCK> PADDING{-128, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 0x80 = -128
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

        public:
            struct Context {
                std::array<uint32_t, 4> state;
                std::array<uint8_t, CBLOCK> buffer;
                uint32_t nl, nh;

                constexpr Context() noexcept;

                constexpr void append(const char* data, std::size_t len) noexcept;

                constexpr void transform(const std::array<uint32_t, LBLOCK>& block) noexcept;

                constexpr std::array<uint8_t, 16> final() noexcept;
            };
        };
    }  // namespace detail

    template<std::size_t N>
    constexpr std::array<uint8_t, 16> md5(const char (&data)[N]) noexcept;

    template<std::size_t N>
    constexpr std::array<uint8_t, 16> md5(const std::array<char, N>& data) noexcept;

    constexpr std::array<uint8_t, 16> md5(const char* s) noexcept;
}  // namespace minecraft

#include "md5.hpp"

#endif  // MD5_H
