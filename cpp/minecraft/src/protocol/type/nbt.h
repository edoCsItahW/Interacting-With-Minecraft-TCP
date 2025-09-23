// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file nbt.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/09/20 23:31
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef NBT_H
#define NBT_H
#pragma once



namespace minecraft::protocol {

    struct NBT {

        [[nodiscard]] encodeType encode() const;

        static auto decode(const std::byte* data);

    };

}

#endif  // NBT_H
