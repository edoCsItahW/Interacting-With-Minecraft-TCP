// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file client.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/21 01:13
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef CLIENT_HPP
#define CLIENT_HPP
#pragma once

#include "../protocol/package/definition.h"
#include "logging.h"

namespace minecraft::client {
    inline Client::Client(std::string ip, const short port, const bool debug)
        : ClientBase(std::move(ip), port, debug) {}

    inline void Client::handleRecv(std::vector<std::byte>& msg, const std::size_t size) const {
        // auto packet = protocol::PackageImpl<>::deserialize(msg.data());
        protocol::parsePacket(state, msg, []<protocol::isPackage T>(const T& packet) {
            if constexpr (std::is_same_v<T, protocol::server_bound::login_step::CompressionPacketType<>>)
                packet.onField("threshold", [&]<protocol::isField F>(const F& field){
                    std::cout << field.toString() << std::endl;
                });
            networkInfo<TO_CLIENT>(packet.toString());
        });
    }

    inline std::vector<std::byte> Client::castChar2T(char* msg, const std::size_t size) const {
        std::vector<std::byte> bytes;
        bytes.reserve(size);

        for (std::size_t i = 0; i < size; i++) bytes.push_back(static_cast<std::byte>(msg[i]));

        return bytes;
    }

    inline char* Client::castT2Char(std::vector<std::byte>& msg, const std::size_t size) const {
        const auto chars = new char[size + 1];

        for (std::size_t i = 0; i < size; i++) chars[i] = static_cast<char>(msg[i]);

        chars[size] = '\0';

        return chars;
    }

    inline void Client::start() {
        using namespace protocol;
        using namespace client_bound;

        const handshake_step::HandShakePacketType hsPacket{VarInt<765>(), String(ip), UShort(port), VarInt<2>()};

        auto hsPacketBytes = hsPacket.serialize();

        msgQueue.emplace(hsPacketBytes, hsPacketBytes.size(), std::nullopt);

        const login_step::LoginStartPacketType lsPacket{String<"edocsitahw">(), UUID<genUUID<"edocsitahw">()>()};

        auto lsPacketBytes = lsPacket.serialize();

        state = State::LOGIN;  // 进入登录状态

        msgQueue.emplace(lsPacketBytes, lsPacketBytes.size(), std::nullopt);

        ClientBase::start();
    }

}  // namespace minecraft::client

#endif  // CLIENT_HPP
