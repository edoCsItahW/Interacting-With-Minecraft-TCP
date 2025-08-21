// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file logging.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/21 17:31
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef LOGGING_HPP
#define LOGGING_HPP
#pragma once

#include "../utils/utils.h"
#include <format>
#include <iostream>

namespace minecraft {
    namespace detail {
        template<LogLevel L>
        void infoCore(std::string message) {
            switch (L) {
                using enum LogLevel;
                case INFO:
                case WARNING: std::cout << '[' << enumToStr(L) << ']' << message << std::endl; break;
                case CRITICAL: std::cerr << '[' << enumToStr(L) << ']' << message << std::endl; break;
            }
        }
    }  // namespace detail

    template<NetDest D, LogLevel L>
    void networkInfo(std::string message) {
        auto dest = D == TO_SERVER ? "->" : "<-";
        detail::infoCore<L>(std::format(" C {} S: {}", dest, message));
    }

    template<FStrChar S, NetDest D, LogLevel L>
    void networkInfo() {
        auto dest = D == TO_SERVER ? "->" : "<-";
        detail::infoCore<L>(std::format(" C {} S: {}", dest, S.data.data()));
    }

    template<LogLevel L>
    void debugInfo(std::string message) {
        detail::infoCore<L>(std::format(" -- : {}", message));
    }

    template<FStrChar S, LogLevel L>
    void debugInfo() {
        detail::infoCore<L>(std::format(" -- : {}", S.data.data()));
    }
}  // namespace minecraft

#endif  // LOGGING_HPP
