// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file clientBase.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/21 00:59
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef CLIENTBASE_HPP
#define CLIENTBASE_HPP
#pragma once

#ifdef _WIN32
    #include <ws2tcpip.h>
#endif

#include <iostream>
#include <thread>

namespace minecraft::client {

    namespace detail {
        static int GLOBAL_WSA_COUNT = 0;

        inline void scoketInit() {
            if (GLOBAL_WSA_COUNT == 0) {
                WSADATA wsaData;

                WSAStartup(MAKEWORD(2, 2), &wsaData);
            }

            GLOBAL_WSA_COUNT++;
        }

        inline void socketClose(const SOCKET sock) {
            closesocket(sock);

            GLOBAL_WSA_COUNT--;

            if (GLOBAL_WSA_COUNT == 0) WSACleanup();
        }
    }  // namespace detail

    template<typename T>
    ClientBase<T>::ClientBase(std::string ip, const short port, bool debug)
        : ip(std::move(ip))
        , port(port)
        , stopFlag(false)
        , debug(debug)
        , closed(false) {
        detail::scoketInit();

        // 将IP地址转换为二进制IP地址
        addrinfo hints{}, *res;
        hints.ai_family   = AF_INET;      // IPv4
        hints.ai_socktype = SOCK_STREAM;  // TCP

        std::string portStr = std::to_string(port);

        int status = getaddrinfo(this->ip.c_str(), portStr.c_str(), &hints, &res);
        if (status != 0) raiseError("Host resolution failed");

        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if (sock == INVALID_SOCKET) raiseError("Socket creation failed");

        debugPrint<"Socket created successfully">();

        // 连接服务器
        if (connect(sock, res->ai_addr, res->ai_addrlen) == SOCKET_ERROR) raiseError("Connection failed");

        debugPrint<"Connected to server successfully">();
    }

    template<typename T>
    ClientBase<T>::~ClientBase() {
        cleanUp();

        debugPrint<"Client stopped">();
    }

    template<typename T>
    void ClientBase<T>::start() {
        recvThread = std::thread(&ClientBase::recvLoop, this);

        sendThread = std::thread(&ClientBase::sendLoop, this);

        recvThread.join();
        debugPrint<"Receive thread joined">();

        sendThread.join();
        debugPrint<"Send thread joined">();
    }

    template<typename T>
    void ClientBase<T>::raiseError(const char* msg) {
        debugPrint<LogLevel::CRITICAL>(std::format("{}: {}", msg, WSAGetLastError()));

        cleanUp();
        exit(1);
    }

    template<typename T>
    void ClientBase<T>::recvLoop() {
        debugPrint<"Receive thread started">();

        while (!stopFlag) {
            int len = recv(sock, recvBuf, sizeof(recvBuf), 0);

            if (len == SOCKET_ERROR)
                raiseError("Receive failed");

            else if (len == 0)
                break;

            auto msg = castChar2T(recvBuf, len);

            handleRecv(msg, len);

            // 防止CPU占用过高，休眠50毫秒
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    template<typename T>
    void ClientBase<T>::sendLoop() {
        debugPrint<"Send thread started">();

        while (!stopFlag) {
            if (!msgQueue.empty()) {
                auto [msg, size, callback] = msgQueue.front();

                msgQueue.pop();

                auto buffer = castT2Char(msg, size);

                if (send(sock, buffer, size, 0) == SOCKET_ERROR) raiseError("Send failed");

                if (callback.has_value()) callback->operator()();

                std::string hexMsg;
                for (std::size_t i = 0; i < size; i++) hexMsg += std::format("\\x{:02x} ", static_cast<unsigned char>(buffer[i]));

                networkInfo<TO_SERVER>(hexMsg);
            }

            // 防止CPU占用过高，休眠100毫秒
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    template<typename T>
    void ClientBase<T>::handleRecv(T& msg, const size_t size) {
        networkInfo<TO_CLIENT>(castT2Char(msg, size));
    }

    template<typename T>
    template<LogLevel L>
    void ClientBase<T>::debugPrint(std::string msg) const {
        if (debug) debugInfo<L>(msg);
    }

    template<typename T>
    template<FStrChar S, LogLevel L>
    void ClientBase<T>::debugPrint() const {
        if (debug) debugInfo<S, L>();
    }

    template<typename T>
    void ClientBase<T>::cleanUp() {
        stopFlag = true;

        if (recvThread.joinable()) recvThread.join();

        if (sendThread.joinable()) sendThread.join();

        if (!closed) {
            closed = true;

            detail::socketClose(sock);
            WSACleanup();
        }
    }

}  // namespace minecraft::client

#endif  // CLIENTBASE_HPP
