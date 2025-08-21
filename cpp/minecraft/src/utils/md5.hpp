// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file md5.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 01:26
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef MD5_HPP
#define MD5_HPP
#pragma once

#include "utils.h"

namespace minecraft {
    namespace detail {
        constexpr auto MD5::makeDigest(const std::array<uint32_t, 4>& state) noexcept {
            std::array<uint8_t, 16> digest{};

            for (std::size_t i{0}; i < state.size(); ++i) {
                digest[i * 4 + 0] = static_cast<uint8_t>(state[i] & 0xFF);
                digest[i * 4 + 1] = static_cast<uint8_t>(state[i] >> 8 & 0xFF);
                digest[i * 4 + 2] = static_cast<uint8_t>(state[i] >> 16 & 0xFF);
                digest[i * 4 + 3] = static_cast<uint8_t>(state[i] >> 24 & 0xFF);
            }

            return digest;
        }

        constexpr uint32_t MD5::calcF(uint32_t b, uint32_t c, uint32_t d) noexcept { return b & c | ~b & d; }

        constexpr uint32_t MD5::calcG(uint32_t b, uint32_t c, uint32_t d) noexcept { return b & d | c & ~d; }

        constexpr uint32_t MD5::calcH(uint32_t b, uint32_t c, uint32_t d) noexcept { return b ^ c ^ d; }

        constexpr uint32_t MD5::calcI(uint32_t b, uint32_t c, uint32_t d) noexcept { return c ^ (b | ~d); }

        constexpr uint32_t MD5::rotate(uint32_t x, uint32_t n) noexcept { return x << n | x >> (32 - n); }

        constexpr uint32_t MD5::calcT(Func f, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) noexcept { return b + rotate(a + f(b, c, d) + x + ac, s); }

        constexpr uint32_t MD5::toUint32(const unsigned char* data) noexcept {
            return static_cast<uint32_t>(data[0]) | static_cast<uint32_t>(data[1]) << 8 | static_cast<uint32_t>(data[2]) << 16 | static_cast<uint32_t>(data[3]) << 24;
        }

        constexpr MD5::Context::Context() noexcept
            : buffer()
            , state{0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476}
            , nl(0)
            , nh(0) {}

        constexpr void MD5::Context::append(const char* data, std::size_t len) noexcept {
            std::array<uint32_t, LBLOCK> input{};

            auto index = (nl >> 3) & 0x3F;
            auto size  = static_cast<uint32_t>(len);

            nl += size << 3;

            if (nl < (size << 3)) nh++;

            nh += size >> 29;

            for (auto ptr = data; ptr != data + len; ++ptr) {
                buffer[index++] = static_cast<uint8_t>(static_cast<int16_t>(*ptr) + UCHAR_MAX + 1);

                if (index == 0x40) {
                    for (auto i = 0, j = 0; i < LBLOCK; ++i, j += 4) input[i] = toUint32(&buffer[j]);

                    transform(input);
                    index = 0;
                }
            }
        }

        constexpr void MD5::Context::transform(const std::array<uint32_t, LBLOCK>& block) noexcept {
            auto a = state[0], b = state[1], c = state[2], d = state[3];

            for (uint32_t i = 0; i < 4; ++i) {
                const auto g = G.data() + i * LBLOCK;
                const auto s = S.data() + i * 4;
                const auto k = K.data() + i * LBLOCK;

                for (auto j = 0; j < block.size(); ++j) {
                    const auto f = calcT(Funcs[i], a, b, c, d, block[g[j]], s[j % 4], k[j]);

                    a = d;
                    d = c;
                    c = b;
                    b = f;
                }
            }

            state[0] += a;
            state[1] += b;
            state[2] += c;
            state[3] += d;
        }

        constexpr std::array<uint8_t, 16> MD5::Context::final() noexcept {
            std::array<uint32_t, LBLOCK> block{};

            auto index = (nl >> 3) & 0x3F;

            block[14] = nl;
            block[15] = nh;

            append(PADDING.data(), index < 56 ? 56 - index : 120 - index);

            for (auto i = 0, j = 0; i < 14; ++i, j += 4) block[i] = toUint32(&buffer[j]);

            transform(block);

            return makeDigest(state);
        }

    }  // namespace detail

    template<std::size_t N>
    constexpr std::array<uint8_t, 16> md5(const char (&data)[N]) noexcept {
        detail::MD5::Context ctx;

        ctx.append(data, N - 1);

        return ctx.final();
    }

    template<std::size_t N>
    constexpr std::array<uint8_t, 16> md5(const std::array<char, N>& data) noexcept {
        detail::MD5::Context ctx;

        ctx.append(data.data(), N);

        return ctx.final();
    }

    constexpr std::array<uint8_t, 16> md5(const char* s) noexcept {
        detail::MD5::Context ctx;

        ctx.append(s, strLen(s));

        return ctx.final();
    }

}  // namespace minecraft

#endif  // MD5_HPP
