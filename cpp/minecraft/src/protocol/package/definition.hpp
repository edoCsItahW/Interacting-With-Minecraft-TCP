// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file definition.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/21 19:24
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef DEFINITION_HPP
#define DEFINITION_HPP
#pragma once

#include "../type/varNum.h"
#include "definition.h"

namespace minecraft::protocol {

    namespace detail {
        template<typename F>
        void parseKnownPacket(const State state, const std::vector<std::byte>& data, bool compress, const F& f) {
            auto dataPtr = data.data();

            auto [totalLength, bytesRead] = detail::parseVarInt<int>(dataPtr);
            dataPtr += bytesRead;

            auto [packetID, shift] = detail::parseVarInt<int>(dataPtr);
            dataPtr -= bytesRead;  // 还原指针位置

            try {
                switch (state) {
                    using enum State;
                    case HANDSHAKE: parseHandshakePacket<F>(packetID, data, compress, f); break;
                    case STATUS: parseStatusPacket<F>(packetID, data, compress, f); break;
                    case LOGIN: parseLoginPacket<F>(packetID, data, compress, f); break;
                    case PLAY: parsePlayPacket<F>(packetID, data, compress, f); break;
                }
            } catch (const std::exception& e) { throw std::runtime_error("Error parsing packet: " + std::string(e.what())); }
        }

        template<typename F>
        void parseHandshakePacket(int, const std::vector<std::byte>&, bool, const F&) {
            throw std::runtime_error("Not packet state found for handshake packet");
        }

        template<typename F>
        void parseStatusPacket(const int id, const std::vector<std::byte>& data, bool compress, const F& f) {
            using namespace server_bound::status_step;
            switch (id) {
                case 0x00: f(ResponsePacketType::deserialize(data.data(), compress)); break;
                case 0x01: f(PongPacketType::deserialize(data.data(), compress)); break;
                default: throw std::runtime_error("Unknown status packet id: " + std::to_string(id));
            }
        }

        template<typename F>
        void parseLoginPacket(const int id, const std::vector<std::byte>& data, bool compress, const F& f) {
            using namespace server_bound::login_step;
            switch (id) {
                case 0x00: f(LoginSuccessPacketType::deserialize(data.data(), compress)); break;
                case 0x03: f(CompressionPacketType::deserialize(data.data(), compress)); break;
                default: throw std::runtime_error("Unknown login packet id: " + std::to_string(id));
            }
        }

        template<typename F>
        void parsePlayPacket(const int id, const std::vector<std::byte>& data, bool compress, const F& f) {
            throw std::runtime_error("Not packet state found for play packet");
        }

        template<typename F>
        void parseUnknownPacket(const std::vector<std::byte>& data, bool compress, const F& f) {
            f(Package<>::deserialize(data.data(), compress));
        }

    }  // namespace detail

    template<typename F>
    void parsePacket(const State state, const std::vector<std::byte>& data, bool compress, const F& f) {
        try {
            detail::parseKnownPacket<F>(state, data, compress, f);
        } catch (const std::exception&) { detail::parseUnknownPacket<F>(data, compress, f); }
    }

}  // namespace minecraft::protocol

#endif  // DEFINITION_HPP
