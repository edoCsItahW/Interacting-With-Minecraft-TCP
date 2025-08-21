// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file logging.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/21 16:19
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef LOGGING_H
#define LOGGING_H
#pragma once

#include "../utils/fstr.h"
#include <string>

namespace minecraft {
    enum class LogLevel { INFO, WARNING, CRITICAL };

    enum NetDest { TO_SERVER, TO_CLIENT };

    template<NetDest D, LogLevel L = LogLevel::INFO>
    void networkInfo(std::string message);

    template<FStrChar S, NetDest D, LogLevel L = LogLevel::INFO>
    void networkInfo();

    template<LogLevel L = LogLevel::INFO>
    void debugInfo(std::string message);

    template<FStrChar S, LogLevel L = LogLevel::INFO>
    void debugInfo();
}  // namespace minecraft

#include "logging.hpp"

#endif  // LOGGING_H
