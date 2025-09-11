// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file definition.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/20 22:03
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef DEFINITION_H
#define DEFINITION_H
#pragma once

#include "../type/varNum.h"
#include "package.h"
#include <functional>

namespace minecraft::protocol {
    enum class State { HANDSHAKE, STATUS, LOGIN, PLAY };

    /** Clientbound packets [Client -> Server] */

    namespace client_bound {
        // Handshake Step

        namespace handshake_step {

            using HandShakePacketType = Package<0, FieldItem<"ProtocolVersion", VarInt>, FieldItem<"ServerAddress", String>, FieldItem<"ServerPort", UShort>, FieldItem<"NextState", VarInt>>;

            using PingPacketType = Package<1, FieldItem<"Payload", UByte>>;

        }  // namespace handshake_step

        // Status Step

        namespace status_step {
            using RequestPacketType = Package<0>;

            using PingPacketType = Package<1, FieldItem<"Payload", Long>>;
        }  // namespace status_step

        // Login Step

        namespace login_step {
            using LoginStartPacketType = Package<0, FieldItem<"Name", String>, FieldItem<"UUID", UUID>>;

            // TODO: Implement CompressionPacketType

        }  // namespace login_step

    }  // namespace client_bound

    /** Serverbound packets [Server -> Client] */

    namespace server_bound {
        namespace status_step {
            using ResponsePacketType = Package<0, FieldItem<"JSON", String>>;

            using PongPacketType = Package<1, FieldItem<"Payload", Long>>;
        }  // namespace status_step

        namespace login_step {
            using DisconnectPacketType = Package<0, FieldItem<"Reason", String>>;

            // TODO: Implement EncryptionRequestPacketType

            using LoginSuccessPacketType = Package<2, FieldItem<"UUID", UUID>, FieldItem<"Username", String>>;

            using CompressionPacketType = Package<3, FieldItem<"Threshold", VarInt>>;

            // using PluginRequestPacketType = Package<4, FieldItem<"MessageID", VarInt>, FieldItem<"Channel", String>>

        }  // namespace login_step
    }  // namespace server_bound

    /* ------------------------ OTHER ------------------------ */

    namespace detail {
        template<typename F>
        void parseKnownPacket(State state, const std::vector<std::byte>& data, bool compress, const F& f);

        template<typename F>
        void parseHandshakePacket(int, const std::vector<std::byte>&, bool compress, const F&);

        template<typename F>
        void parseStatusPacket(int id, const std::vector<std::byte>& data, bool compress, const F& f);

        template<typename F>
        void parseLoginPacket(int id, const std::vector<std::byte>& data, bool compress, const F& f);

        template<typename F>
        void parsePlayPacket(int id, const std::vector<std::byte>& data, bool compress, const F& f);

        template<typename F>
        void parseUnknownPacket(const std::vector<std::byte>& data, bool compress, const F& f);
    }  // namespace detail

    template<typename F>
    void parsePacket(State state, const std::vector<std::byte>& data, bool compress, const F& f);

}  // namespace minecraft::protocol

#include "definition.hpp"

#endif  // DEFINITION_H
