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
#include <any>

#ifdef DEBUG
    #include "../utils/debugger.h"
#endif

namespace minecraft::client {

    inline Client::Client(std::string ip, const short port, const bool debug)
        : ClientBase(std::move(ip), port, debug) {
        using namespace protocol;
        namespace svr = server_bound;
        namespace cli = client_bound;

        on<svr::login_step::CompressionPacketType>([this](const auto& packet) {
            if (auto t = packet.template get<"Threshold">().value(); t >= 0) {
                compress  = true;
                threshold = t;
            }
        });

        on<svr::login_step::LoginSuccessPacketType>([this](const auto&) {
            state = State::PLAY;

            emit(cli::login_step::LoginConfirmPacketType{});
        });

        on<svr::play_step::SpawnEntityPacketType>([this](const auto&) { emit(cli::configuration_step::FinishConfigurationPacketType{}); });

        on<svr::play_step::SynchronizePlayerPositionPacketType>([this](const auto& packet) { emit(cli::play_step::TeleportConfirmPacketType{packet.template get<"TeleportID">()}); });

        on<svr::play_step::KeepAlivePacketType>([this](const auto& packet) { emit(cli::play_step::KeepAlivePacketType{packet.template get<"KeepAliveID">()}); });
    }

    template<protocol::is_package T>
    void Client::on(std::function<void(const T&)> callback, int times) {
        auto& vec = packageCallbacks[typeid(T)];

        vec.push_back(std::make_pair(times, [callback](const std::any& packet) { callback(std::any_cast<const T&>(packet)); }));
    }

    template<protocol::State S, int I>
    void Client::on(std::function<void(const typename protocol::ServerPacketType<S, I>::type&)> callback, const int times) {
        using T = typename protocol::ServerPacketType<S, I>::type;

        on<T>(callback, times);
    }

    template<protocol::is_package T>
    void Client::once(std::function<void(const T&)> callback) {
        on<T>(callback, 1);
    }

    template<protocol::State S, int I>
    void Client::once(std::function<void(const typename protocol::ServerPacketType<S, I>::type&)> callback) {
        on<S, I>(callback, 1);
    }

    template<protocol::is_package T>
    void Client::emit(T&& package, std::optional<std::function<void()>> callback) {
        auto packetBytes = package.serialize(compress, threshold);

        msgQueue.emplace(packetBytes, packetBytes.size(), callback);
    }

    inline void Client::handleRecv(std::vector<std::byte>& msg, const std::size_t size) {
        using namespace protocol;

        auto cb = [this]<is_package T>(const T& packet) {
            if (const auto it = packageCallbacks.find(typeid(T)); it != packageCallbacks.end())
                for (auto& [times, callback] : it->second) {
                    if (times != 0) callback(std::any(packet));

                    if (times > 0) --times;
                }

            networkInfo<TO_CLIENT>(std::format("[{}] {}", enumToStr(state), packet.toString()));
        };

#ifdef DEBUG
        Debugger parsePacketDbg(&parsePacket<decltype(cb)>);
        parsePacketDbg
#else

        parsePacket

#endif
            (state, msg, compress, cb);
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

        emit(handshake_step::HandShakePacketType{VarInt(765), String(ip), UShort(port), VarInt(2)});

        emit(login_step::LoginStartPacketType{String("edocsitahw"), UUID(genUUID<"edocsitahw">())}, [this] { state = State::LOGIN; });

        ClientBase::start();
    }

}  // namespace minecraft::client

#endif  // CLIENT_HPP
