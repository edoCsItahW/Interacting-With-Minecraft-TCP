// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file uuid.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 17:08
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef UUID_H
#define UUID_H
#pragma once

#include <array>
#include <cstdint>

namespace minecraft {
    namespace detail {
        class UUID {
        public:
            template<std::size_t N>
            static constexpr std::array<uint8_t, 16> generate(const char (&str)[N]);

            template<std::size_t N>
            static constexpr std::array<uint8_t, 16> generate(const std::array<uint8_t, N>& arr);

        private:
            static constexpr std::array<uint8_t, 16> NAMESPACE_NIL = {};
        };
    }  // namespace detail

    template<std::size_t N>
    constexpr auto uuid(const char (&data)[N]);

    template<std::size_t N>
    constexpr auto uuid(const std::array<uint8_t, N>& data);

    constexpr auto uuidToArr(const std::array<uint8_t, 16>& uuid) -> std::array<char, 36>;

    constexpr auto uuidToStr(const std::array<uint8_t, 16>& uuid);
}  // namespace minecraft

#include "uuid.hpp"

#endif  // UUID_H
