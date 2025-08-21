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
#define HANDSHAKE_TYPES_C_HK  VarIntType, StringType, UShortType, VarIntType

#define HANDSHAKE_FIELDS_C_HK "ProtocolVersion"_s, "ServerAddress"_s, "ServerPort"_s, "NextState"_s

            using HandShakeTypes = TypeArgs<HANDSHAKE_TYPES_C_HK>;

            using HandShakePacketType = Package<0, HANDSHAKE_FIELDS_C_HK>;

#define PING_TYPES_C_HK  VarIntType

#define PING_FIELDS_C_HK "Payload"_s

            using PingTypes = TypeArgs<PING_TYPES_C_HK>;

            using PingPacketType = Package<1, PING_FIELDS_C_HK>;

            inline PingPacketType PingPacket{UByte<0>()};
        }  // namespace handshake_step

        // Status Step

        namespace status_step {
            using RequestPacketType = Package<0>;

#define PING_TYPES_S_ST  LongType

#define PING_FIELDS_S_ST "Payload"_s

            using PingTypes = TypeArgs<PING_TYPES_S_ST>;

            using PingPacketType = Package<1, PING_FIELDS_S_ST>;
        }  // namespace status_step

        // Login Step

        namespace login_step {
#define LOGIN_START_TYPES_C_LG  StringType, UUIDType

#define LOGIN_START_FIELDS_C_LG "Name"_s, "UUID"_s

            using LoginStartTypes = TypeArgs<LOGIN_START_TYPES_C_LG>;

            using LoginStartPacketType = Package<0, LOGIN_START_FIELDS_C_LG>;

            // TODO: Implement CompressionPacketType

        }  // namespace login_step
    }  // namespace client_bound

    /** Serverbound packets [Server -> Client] */

    namespace server_bound {
        namespace status_step {
#define RESPONSE_TYPES_S_ST  StringType

#define RESPONSE_FIELDS_S_ST "JSON"_s

            using ResponseTypes = TypeArgs<RESPONSE_TYPES_S_ST>;

            using ResponsePacketType = Package<0, RESPONSE_FIELDS_S_ST>;

#define PONG_TYPES_S_ST  LongType

#define PONG_FIELDS_S_ST "Payload"_s

            using PongTypes = TypeArgs<PONG_TYPES_S_ST>;

            using PongPacketType = Package<1, PONG_FIELDS_S_ST>;
        }  // namespace status_step

        namespace login_step {
#define DISCONNECT_TYPES_S_LG     StringType

#define DISCONNECT_FIELDS_S_LG    "Reason"_s

            // TODO: Implement DisconnectPacketType

            // TODO: Implement EncryptionRequestPacketType

#define LOGIN_SUCCESS_TYPES_S_LG  UUIDType, StringType

#define LOGIN_SUCCESS_FIELDS_S_LG "UUID"_s, "Username"_s

            using LoginSuccessTypes = TypeArgs<LOGIN_SUCCESS_TYPES_S_LG>;

            using LoginSuccessPacketType = Package<0, LOGIN_SUCCESS_FIELDS_S_LG>;

#define COMPRESSION_TYPES_S_LG  VarIntType

#define COMPRESSION_FIELDS_S_LG "Threshold"_s

            using CompressionTypes = TypeArgs<COMPRESSION_TYPES_S_LG>;

            using CompressionPacketType = Package<3, COMPRESSION_FIELDS_S_LG>;

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
