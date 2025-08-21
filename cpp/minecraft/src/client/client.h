// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file client.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/21 01:13
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef CLIENT_H
#define CLIENT_H
#pragma once

#include "../protocol/package/definition.h"
#include "clientBase.h"

namespace minecraft::client {

    class Client final : public ClientBase<std::vector<std::byte>> {
    public:
        explicit Client(std::string ip = "127.0.0.1", short port = 25565, bool debug = false);

        void start() override;

    private:
        protocol::State state = protocol::State::HANDSHAKE;

        void handleRecv(std::vector<std::byte>& msg, std::size_t size) const override;

        std::vector<std::byte> castChar2T(char* msg, std::size_t size) const override;

        char* castT2Char(std::vector<std::byte>& msg, std::size_t size) const override;
    };
}  // namespace minecraft::client

#include "client.hpp"

#endif  // CLIENT_H
