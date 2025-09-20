// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file identifier.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/14 11:18
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef IDENTIFIER_H
#define IDENTIFIER_H
#pragma once

#include "str.h"

namespace minecraft::protocol {

    struct Identifier : String {
        using type = std::string;

        using serializeType = std::vector<std::byte>;

        Identifier();

        Identifier(const std::string& str);

        static auto deserialize(const std::byte* data);
    };

    template<typename T>
    concept is_identifier_field = std::is_same_v<T, Identifier>;

}  // namespace minecraft::protocol

#include "identifier.hpp"

#endif  // IDENTIFIER_H
