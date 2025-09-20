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

#include "../type/mcarray.h"
#include "../type/varNum.h"
#include "definition.h"
#include "package.h"

namespace minecraft::protocol {
    enum class State { HANDSHAKE, STATUS, LOGIN, CONFIGURATION, PLAY };

    /** Clientbound packets [Client -> Server] */

    template<State S, int I>
    struct ClientPacketType;

#ifdef DEFINITION_H
    template<FStrChar V, is_field T, detail::is_nullable_fstr auto D = Null>
    using FI = FieldItem<V, T, D>;
#endif

    template<FStrChar V>
    constexpr auto operator""_ns() {
        return Nullable{V};
    }

    namespace client_bound {
        // Handshake Step

        namespace handshake_step {

            using HandShakePacketType = Package<0, FI<"ProtocolVersion", VarInt>, FI<"ServerAddress", String>, FI<"ServerPort", UShort>, FI<"NextState", VarInt>>;

            using PingPacketType = Package<1, FI<"Payload", UByte>>;

        }  // namespace handshake_step

        // Status Step

        namespace status_step {

            using RequestPacketType = Package<0>;

            using PingPacketType = Package<1, FI<"Payload", Long>>;

        }  // namespace status_step

        // Configuration Step

        namespace configuration_step {

            using FinishConfigurationPacketType = Package<2>;

        }  // namespace configuration_step

        // Login Step

        namespace login_step {

            using LoginStartPacketType = Package<0, FI<"Name", String>, FI<"UUID", UUID>>;

            using EncryptionResponsePacketType =
                Package<1, FI<"SharedSecretLength", VarInt>, FI<"SharedSecret", Array<>, "SharedSecretLength"_ns>, FI<"VerifyTokenLength", VarInt>, FI<"VerifyToken", Array<>, "VerifyTokenLength"_ns>>;

            using LoginPluginRequestPacketType = Package<2, FI<"MessageID", VarInt>, FI<"Successful", Boolean>, FI<"Data", Array<>, "__rest__"_ns>>;

            using LoginConfirmPacketType = Package<3>;

        }  // namespace login_step

        namespace play_step {

            using TeleportConfirmPacketType = Package<0, FI<"TeleportID", VarInt>>;

            using KeepAlivePacketType = Package<16, FI<"KeepAliveID", Long>>;

        }

    }  // namespace client_bound

    template<>
    struct ClientPacketType<State::HANDSHAKE, 0> {
        using type = client_bound::handshake_step::HandShakePacketType;
    };

    template<>
    struct ClientPacketType<State::HANDSHAKE, 1> {
        using type = client_bound::handshake_step::PingPacketType;
    };

    /** Serverbound packets [Server -> Client] */

    template<State S, int I>
    struct ServerPacketType;

    namespace server_bound {
        namespace status_step {

            using ResponsePacketType = Package<0, FI<"JSON", String>>;

            using PongPacketType = Package<1, FI<"Payload", Long>>;

        }  // namespace status_step

        namespace login_step {

            using DisconnectPacketType = Package<0, FI<"Reason", String>>;

            using EncryptionRequestPacketType = Package<
                1, FI<"ServerID", String>, FI<"PublicKeyLength", VarInt>, FI<"PublicKey", Array<>, "PublicKeyLength"_ns>, FI<"VerifyTokenLength", VarInt>,
                FI<"VerifyToken", Array<>, "VerifyTokenLength"_ns>>;

            using LoginSuccessPacketType = Package<2, FI<"UUID", UUID>, FI<"Username", String>>;

            using CompressionPacketType = Package<3, FI<"Threshold", VarInt>>;

            using PluginRequestPacketType = Package<4, FI<"MessageID", VarInt>, FI<"Channel", String>, FI<"Data", Array<>, "__rest__"_ns>>;

        }  // namespace login_step

        namespace play_step {

            using SpawnEntityPacketType = Package<
                0, FI<"EntityID", VarInt>, FI<"UUID", UUID>, FI<"Type", VarInt>, FI<"X", Double>, FI<"Y", Double>, FI<"Z", Double>, FI<"Pitch", Angle>, FI<"Yaw", Angle>, FI<"Data", Int>,
                FI<"VelocityX", Short>, FI<"VelocityY", Short>, FI<"VelocityZ", Short>>;

            using SpawnExperienceOrbPacketType = Package<1, FI<"EntityID", VarInt>, FI<"X", Double>, FI<"Y", Double>, FI<"Z", Double>, FI<"Count", Short>>;

            using ChangeDifficultyPacketType = Package<11, FI<"Difficulty", UByte>, FI<"DifficultyLocked", Boolean>>;

            // 0x1B
            using DisconnectPacketType = Package<27, FI<"Reason", String>>;

            using KeepAlivePacketType = Package<36, FI<"KeepAliveID", Long>>;

            // 0x58
            using SetEntityVelocityPacketType = Package<38, FI<"EntityID", VarInt>, FI<"VelocityX", Short>, FI<"VelocityY", Short>, FI<"VelocityZ", Short>>;

            // 0x2B
            using LoginPacketType = Package<
                41, FI<"EntityID", Int>, FI<"IsHardcore", Boolean>, FI<"DimensionNames", PrefixedArray<Identifier>>, FI<"MaxPlayers", VarInt>, FI<"ViewDistance", VarInt>,
                FI<"SimulationDistance", VarInt>, FI<"ReducedDebugInfo", Boolean>, FI<"EnableRespawnScreen", Boolean>, FI<"DoLimitedCrafting", Boolean>, FI<"DimensionType", VarInt>,
                FI<"DimensionName", Identifier>, FI<"HashedSeed", Long>, FI<"GameMode", UByte>, FI<"PreviousGameMode", Byte>, FI<"IsDebug", Boolean>, FI<"IsFlat", Boolean>,
                FI<"HasDeathLocation", Boolean>, FI<"DeathDimensionName", Option<Identifier>, "HasDeathLocation"_ns>, FI<"DeathLocation", Option<Position>, "HasDeathLocation"_ns>,
                FI<"PortalCooldown", VarInt>, FI<"SeaLevel", VarInt>, FI<"EnforcesSecureChat", Boolean>>;

            // 0x5A
            using SetExpressionPacketType = Package<44, FI<"ExperienceBar", Float>, FI<"Level", VarInt>, FI<"TotalExperience", VarInt>>;

            using SpawnPlayerPacketType = Package<60, FI<"EntityID", VarInt>, FI<"UUID", UUID>, FI<"X", Double>, FI<"Y", Double>, FI<"Z", Double>, FI<"Yaw", Angle>, FI<"Pitch", Angle>>;

            using SpawnEntity2PacketType = Package<
                62, FI<"EntityID", VarInt>, FI<"UUID", UUID>, FI<"Type", VarInt>, FI<"X", Double>, FI<"Y", Double>, FI<"Z", Double>, FI<"Pitch", Angle>, FI<"Yaw", Angle>, FI<"HeadYaw", Angle>,
                FI<"Data", VarInt>, FI<"VelocityX", Short>, FI<"VelocityY", Short>, FI<"VelocityZ", Short>>;

            // 0x5D
            // using SetPassengersPacketType = Package<86, FI<"EntityID", VarInt>, FI<"PassengerCount", VarInt>, FI<"PPassengers", Array<VarInt>, "PassengerCount"_ns>>;
            using SetEntityMetadataPacketType = Package<86, FI<"EntityID", VarInt>, FI<"Metadata", CompoundArray<Byte, VarInt, >>>;

            // 0x47
            using UpdateSectionBlocksPacketType = Package<88, FI<"ChunkSectionPostion", Long>, FI<"BlocksArraySize", VarInt>, FI<"Blocks", Array<VarLong>, "BlocksArraySize"_ns>>;

            // 0x3E
            using SynchronizePlayerPositionPacketType =
                Package<98, FI<"X", Double>, FI<"Y", Double>, FI<"Z", Double>, FI<"Yaw", Float>, FI<"Pitch", Float>, FI<"Flags", Byte>, FI<"TeleportID", VarInt>>;

            // 0x73
            using UpdateRecipesPacketType = Package<102, FI<"NumRecipes", VarInt>, FI<"Recipe", Array<Identifier>, "NumRecipes"_ns>>;

        }  // namespace play_step
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
        void parseConfigurationPacket(int id, const std::vector<std::byte>& data, bool compress, const F& f);

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
