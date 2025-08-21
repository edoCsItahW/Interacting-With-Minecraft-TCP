// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file clientBase.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/21 00:25
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef CLIENTBASE_H
#define CLIENTBASE_H
#pragma once

#ifdef _WIN32
    #include <winsock2.h>

    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <sys/socket.h>
#endif

#include "logging.h"
#include <atomic>
#include <optional>
#include <queue>
#include <thread>

namespace minecraft::client {

    namespace detail {
        void scoketInit();

        void socketClose(SOCKET sock);
    }  // namespace detail

    template<typename T>
    class ClientBase {
    public:
        ClientBase(std::string ip, short port, bool debug = false);

        virtual ~ClientBase();

        virtual void start();

        std::string ip;

        short port;

    protected:
        SOCKET sock;

        std::queue<std::tuple<T, std::size_t, std::optional<std::function<void()>>>> msgQueue;

        char recvBuf[1024];

        std::atomic_bool stopFlag;

        bool debug;

        bool closed;

        std::thread recvThread;

        std::thread sendThread;

        virtual char* castT2Char(T& msg, std::size_t size) const = 0;

        virtual T castChar2T(char* msg, std::size_t size) const = 0;

        virtual void handleRecv(T& msg, std::size_t size) const;

        void recvLoop();

        void sendLoop();

        void raiseError(const char* msg);

        template<LogLevel L = LogLevel::INFO>
        void debugPrint(std::string msg) const;

        template<FStrChar S, LogLevel L = LogLevel::INFO>
        void debugPrint() const;

        void cleanUp();
    };
}  // namespace minecraft::client

#include "clientBase.hpp"

#endif  // CLIENTBASE_H
