// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file position.h 
 * @author edocsitahw 
 * @version 1.1
 * @date 2025/09/13 17:06
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef POSITION_H
#define POSITION_H
#pragma once

#include <array>
#include <tuple>
#include <string>

namespace minecraft::protocol {
    struct Position {
    private:
        static constexpr std::size_t size_ = 8;

        std::tuple<int64_t, int64_t, int64_t> value_;

        mutable bool cached = false;

        mutable std::array<std::byte, size_> data{};

        int64_t x_ = 0;
        int64_t y_ = 0;
        int64_t z_ = 0;

    public:
        using type = std::tuple<int64_t, int64_t, int64_t>;

        using serializeType = std::array<std::byte, size_>;

        Position() = default;

        Position(int64_t x, int64_t y, int64_t z);

        Position(const type& value);

        [[nodiscard]] static std::size_t size();

        [[nodiscard]] type value() const;

        [[nodiscard]] serializeType serialize() const;

        static auto deserialize(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<typename T>
    concept is_position_field = std::is_same_v<T, Position>;

}

#include "position.hpp"

#endif //POSITION_H
