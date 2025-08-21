// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file sha1.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 17:04
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef SHA1_H
#define SHA1_H
#pragma once

#include "fstr.h"
#include <array>
#include <string_view>

namespace minecraft {
    namespace detail {
        /** @class SHA1
         *
         * @if zh
         *
         * @brief 支持运行期和编译期计算的SHA1哈希算法实现。
         *
         * @else
         *
         * @brief SHA1 hash algorithm implementation that supports both runtime and compile-time computation.
         *
         * @endif
         */
        class SHA1 {
        public:
            constexpr SHA1() noexcept;

            constexpr void reset() noexcept;

            constexpr void update(std::string_view data) noexcept;

            constexpr void update(const uint8_t* data, std::size_t size) noexcept;

            constexpr std::array<uint8_t, 20> final() noexcept;

            template<std::size_t N>
            static constexpr std::array<uint8_t, 20> compute(const char (&data)[N]) noexcept;

            template<std::size_t N>
            static constexpr std::array<uint8_t, 20> compute(const std::array<uint8_t, N>& data) noexcept;

            template<std::size_t N>
            static constexpr std::array<uint8_t, 20> compute(const std::array<char, N>& data) noexcept;

            static constexpr std::array<uint8_t, 20> compute(const char* data, size_t len) noexcept;

        private:
            static constexpr std::array<uint8_t, 20> computeImpl(const char* data, std::size_t size) noexcept;

            static constexpr std::array<uint8_t, 20> computeImpl(const uint8_t* data, std::size_t size) noexcept;

            constexpr void addPadding() noexcept;

            constexpr void processBlock(const uint8_t* block) noexcept;

            static constexpr uint32_t rotateLeft(uint32_t value, uint32_t shift) noexcept;

            static constexpr std::size_t blockSize = 64;

            std::array<uint32_t, 5> H;
            std::array<uint8_t, 128> buffer;
            std::size_t bufferSize;
            uint64_t totalBits;
        };

    }  // namespace detail

    template<std::size_t N>
    constexpr auto sha1(const char (&data)[N]);

    template<std::size_t N>
    constexpr auto sha1(const std::array<uint8_t, N>& data);

    template<FStr V>
    constexpr auto sha1();
}  // namespace minecraft

#include "sha1.hpp"

#endif  // SHA1_H
