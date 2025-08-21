// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file sha1.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 01:19
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef SHA1_HPP
#define SHA1_HPP
#pragma once

namespace minecraft {
    namespace detail {

        constexpr SHA1::SHA1() noexcept
            : bufferSize(0)
            , totalBits(0) {
            reset();
        }

        constexpr void SHA1::reset() noexcept {
            H = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
            buffer.fill(0);
            bufferSize = 0;
            totalBits  = 0;
        }

        constexpr void SHA1::update(std::string_view data) noexcept { update(reinterpret_cast<const uint8_t*>(data.data()), data.size()); }

        constexpr void SHA1::update(const uint8_t* data, std::size_t size) noexcept {
            totalBits += size * 8;

            while (size > 0) {
                const std::size_t available = blockSize - bufferSize;
                const std::size_t toCopy    = std::min(available, size);

                for (size_t i = 0; i < toCopy; ++i) buffer[bufferSize + i] = data[i];

                bufferSize += toCopy;
                data += toCopy;
                size -= toCopy;

                if (bufferSize == blockSize) {
                    processBlock(buffer.data());
                    bufferSize = 0;
                }
            }
        }

        constexpr std::array<uint8_t, 20> SHA1::final() noexcept {
            auto savedH          = H;
            auto savedBuffer     = buffer;
            auto savedBufferSize = bufferSize;
            auto savedTotalBits  = totalBits;

            addPadding();

            while (bufferSize >= blockSize) {
                processBlock(buffer.data());
                bufferSize -= blockSize;

                std::copy_n(buffer.begin() + blockSize, bufferSize, buffer.begin());
            }

            std::array<uint8_t, 20> digest{};
            for (std::size_t i{0}; i < 5; ++i) {
                digest[i * 4]     = static_cast<uint8_t>(H[i] >> 24);
                digest[i * 4 + 1] = static_cast<uint8_t>(H[i] >> 16);
                digest[i * 4 + 2] = static_cast<uint8_t>(H[i] >> 8);
                digest[i * 4 + 3] = static_cast<uint8_t>(H[i]);
            }

            H          = savedH;
            buffer     = savedBuffer;
            bufferSize = savedBufferSize;
            totalBits  = savedTotalBits;

            return digest;
        }

        constexpr std::array<uint8_t, 20> SHA1::computeImpl(const char* data, std::size_t size) noexcept {
            SHA1 sha1;
            sha1.update(reinterpret_cast<const uint8_t*>(data), size);
            return sha1.final();
        }

        constexpr std::array<uint8_t, 20> SHA1::computeImpl(const uint8_t* data, std::size_t size) noexcept {
            SHA1 sha1;
            sha1.update(data, size);
            return sha1.final();
        }

        template<std::size_t N>
        constexpr std::array<uint8_t, 20> SHA1::compute(const char (&data)[N]) noexcept {
            return computeImpl(data, N - 1);
        }

        template<std::size_t N>
        constexpr std::array<uint8_t, 20> SHA1::compute(const std::array<uint8_t, N>& data) noexcept {
            return computeImpl(data.data(), N);
        }

        constexpr std::array<uint8_t, 20> SHA1::compute(const char* data, size_t len) noexcept {
            SHA1 sha;

            while (len > 0) {
                const std::size_t chunkSize = std::min(len, blockSize - sha.bufferSize);

                for (std::size_t i = 0; i < chunkSize; ++i) sha.buffer[sha.bufferSize + i] = static_cast<uint8_t>(data[i]);

                data += chunkSize;  // 修复：移动数据指针
                sha.bufferSize += chunkSize;
                sha.totalBits += chunkSize * 8;  // 修复：累积总比特数
                len -= chunkSize;

                if (sha.bufferSize == blockSize) {
                    sha.processBlock(sha.buffer.data());
                    sha.bufferSize = 0;
                }
            }

            return sha.final();
        }

        template<std::size_t N>
        constexpr std::array<uint8_t, 20> SHA1::compute(const std::array<char, N>& data) noexcept {
            return compute(data.data(), N);
        }

        constexpr void SHA1::addPadding() noexcept {
            buffer[bufferSize++] = 0x80;

            std::size_t remaining = blockSize - bufferSize;

            if (remaining < 8) {
                std::fill_n(buffer.begin() + bufferSize, remaining, 0);

                bufferSize = blockSize;
                remaining  = blockSize;
            }

            std::fill_n(buffer.begin() + bufferSize, blockSize - 8, 0);

            bufferSize = blockSize - 8;

            const uint64_t bitLength = totalBits;
            for (std::size_t i = 0; i < 8; ++i) buffer[bufferSize + i] = static_cast<uint8_t>(bitLength >> (56 - 8 * i));

            bufferSize += 8;
        }

        constexpr void SHA1::processBlock(const uint8_t* block) noexcept {
            std::array<uint32_t, 80> W{};

            for (std::size_t i = 0; i < 16; ++i) W[i] = block[i * 4] << 24 | block[i * 4 + 1] << 16 | block[i * 4 + 2] << 8 | block[i * 4 + 3];

            for (std::size_t i = 16; i < 80; ++i) W[i] = rotateLeft(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);

            uint32_t a = H[0], b = H[1], c = H[2], d = H[3], e = H[4];

            for (std::size_t i = 0; i < 80; ++i) {
                uint32_t f, k;

                if (i < 20) {
                    f = b & c | ~b & d;
                    k = 0x5A827999;
                }

                else if (i < 40) {
                    f = b ^ c ^ d;
                    k = 0x6ED9EBA1;
                }

                else if (i < 60) {
                    f = b & c | b & d | c & d;
                    k = 0x8F1BBCDC;
                }

                else {
                    f = b ^ c ^ d;
                    k = 0xCA62C1D6;
                }

                uint32_t temp = rotateLeft(a, 5) + f + e + k + W[i];
                e             = d;
                d             = c;
                c             = rotateLeft(b, 30);
                b             = a;
                a             = temp;
            }

            H[0] += a;
            H[1] += b;
            H[2] += c;
            H[3] += d;
            H[4] += e;
        }

        constexpr uint32_t SHA1::rotateLeft(uint32_t value, uint32_t shift) noexcept { return value << shift | value >> (32 - shift); }
    }  // namespace detail

    template<std::size_t N>
    constexpr auto sha1(const char (&data)[N]) {
        return detail::SHA1::compute(data);
    }

    template<std::size_t N>
    constexpr auto sha1(const std::array<uint8_t, N>& data) {
        return detail::SHA1::compute(data);
    }

    template<FStr V>
    constexpr auto sha1() {
        return detail::SHA1::compute(V.data);
    }
}  // namespace minecraft

#endif  // SHA1_HPP
