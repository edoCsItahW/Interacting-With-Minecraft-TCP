// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file debugger.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/10/10 下午4:42
 * @brief
 * @copyright CC BY-NC-SA
 * */

#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP
#pragma once

#include <iostream>

namespace minecraft {

    template<typename F>
    Debugger<F>::Debugger(F &&func, const bool exit, const std::source_location &loc)
        : func(std::move(func))
        , exit(exit)
        , loc(loc) {}

    template<typename F>
    Debugger<F>::Debugger(F &&func, const std::function<void(std::exception)> &callback, const bool exit, const std::source_location &loc)
        : func(func)
        , callback(callback)
        , exit(exit)
        , loc(loc) {}

    template<typename F>
    template<typename... Args>
    auto Debugger<F>::operator()(Args &&...args) -> decltype(auto) {
        using R = decltype(func(std::forward<Args>(args)...));

        try {
            return func(std::forward<Args>(args)...);

        } catch (const std::exception &e) {
            if (First) {
                std::cerr << "Traceback (most recent call last):" << std::endl;

                First = false;
            }

            std::cerr << "    File " << loc.file_name() << ", line " << loc.line() << ", in <" << loc.function_name() << ">"
                      << "\n\t" << e.what() << std::endl;

            if (callback.has_value()) callback.value()(e);

            if constexpr (!std::is_same_v<R, void>) {
                if (exit) std::terminate();

                return R{};
            }
        }
    }

    template<class C, typename F>
    Debugger<F C::*>::Debugger(F C::*func, const bool exit, const std::source_location &loc)
        : func(func)
        , exit(exit)
        , loc(loc) {}

    template<class C, typename F>
    template<typename... Args>
    auto Debugger<F C::*>::operator()(C *obj, Args &&...args) -> decltype(auto) {
        using R = decltype((obj->*func)(std::forward<Args>(args)...));

        try {
            return (obj->*func)(std::forward<Args>(args)...);

        } catch (const std::exception &e) {
            if (First) {
                std::cerr << "Traceback (most recent call last):" << std::endl;

                First = false;
            }

            std::cerr << "    File " << loc.file_name() << ", line " << loc.line() << ", in <" << loc.function_name() << ">"
                      << "\n\t" << e.what() << std::endl;

            if constexpr (!std::is_same_v<R, void>) {
                if (exit) std::terminate();
                return R{};
            }
        }
    }

}  // namespace minecraft

#endif  // DEBUGGER_HPP
