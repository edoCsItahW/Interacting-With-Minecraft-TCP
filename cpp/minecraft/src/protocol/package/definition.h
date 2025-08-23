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
#include "PackageImpl.h"
#include <functional>

namespace minecraft::protocol {
    enum class State { HANDSHAKE, STATUS, LOGIN, PLAY };

    /** Clientbound packets [Client -> Server] */

    namespace client_bound {
        // Handshake Step

        namespace handshake_step {

            template<varIntField T0, stringField T1, shortField T2, varIntField T3>
            using HandShakePacketType = Package<0, FieldItem<"ProtocolVersion", T0>, FieldItem<"ServerAddress", T1>, FieldItem<"ServerPort", T2>, FieldItem<"NextState", T3>>;

            template<ubyteField T0>
            using PingPacketType = Package<1, FieldItem<"Payload", T0>>;

            inline PingPacketType PingPacket{UByte<0>()};
        }  // namespace handshake_step

        // Status Step

        namespace status_step {
            using RequestPacketType = Package<0>;

            template<longField T0>
            using PingPacketType = Package<1, FieldItem<"Payload", T0>>;
        }  // namespace status_step

        // Login Step

        namespace login_step {
            template<stringField T0, uuidField T1>
            using LoginStartPacketType = Package<0, FieldItem<"Name", T0>, FieldItem<"UUID", T1>>;

            // TODO: Implement CompressionPacketType

        }  // namespace login_step
    }  // namespace client_bound

    /** Serverbound packets [Server -> Client] */

    namespace server_bound {
        namespace status_step {
            template<stringField T0>
            using ResponsePacketType = Package<0, FieldItem<"JSON", T0>>;

            template<longField T0>
            using PongPacketType = Package<1, FieldItem<"Payload", T0>>;
        }  // namespace status_step

        namespace login_step {
            // TODO: Implement DisconnectPacketType

            // TODO: Implement EncryptionRequestPacketType

            template<uuidField T0, stringField T1>
            using LoginSuccessPacketType = Package<0, FieldItem<"UUID", T0>, FieldItem<"Username", T1>>;

            template<varIntField T0>
            using CompressionPacketType = Package<3, FieldItem<"Threshold", T0>>;

            // TODO: Implement PluginRequestPacketType

        }  // namespace login_step
    }  // namespace server_bound

    /* ------------------------ OTHER ------------------------ */

    namespace detail {
        template<typename F>
        void parseKnownPacket(State state, const std::vector<std::byte>& data, const F& f);

        template<typename F>
        void parseHandshakePacket(int, const std::vector<std::byte>&, const F&);

        template<typename F>
        void parseStatusPacket(int id, const std::vector<std::byte>& data, const F& f);

        template<typename F>
        void parseLoginPacket(int id, const std::vector<std::byte>& data, const F& f);

        template<typename F>
        void parsePlayPacket(int id, const std::vector<std::byte>& data, const F& f);

        template<typename F>
        void parseUnknownPacket(const std::vector<std::byte>& data, const F& f);
    }  // namespace detail

    template<typename F>
    void parsePacket(State state, const std::vector<std::byte>& data, const F& f);

}  // namespace minecraft::protocol

#include "definition.hpp"

#endif  // DEFINITION_H
