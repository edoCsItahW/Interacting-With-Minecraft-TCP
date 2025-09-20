// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file identifier.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/14 11:19
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP
#pragma once

namespace minecraft::protocol {

    inline Identifier::Identifier()
        : String() {}

    inline Identifier::Identifier(const std::string& str)
        : String(str) {}

    inline auto Identifier::deserialize(const std::byte* data) { return Identifier(String::deserialize(data).value()); }

}  // namespace minecraft::protocol

#endif  // IDENTIFIER_HPP
