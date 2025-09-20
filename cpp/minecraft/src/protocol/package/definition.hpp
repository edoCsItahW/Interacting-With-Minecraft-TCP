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
            std::vector<std::byte> decompressedData;

            if (compress) {
                auto [packetLen, packetLenShift] = parseVarInt<int>(dataPtr);
                dataPtr += packetLenShift;

                auto [dataLen, dataLenShift] = parseVarInt<int>(dataPtr);
                dataPtr += dataLenShift;
                packetLen -= dataLenShift;

                const auto dataVec = std::vector(dataPtr, dataPtr + packetLen);
                if (dataLen) {
#ifdef DEBUG
                    Debugger decompressDataDbg(&decompressData);
                    decompressedData = decompressDataDbg(dataVec, dataLen);
#else
                    decompressedData = decompressData(dataVec, dataLen);
#endif

                    dataPtr = decompressedData.data();

                    if (dataPtr == nullptr)
                        throw std::runtime_error("Decompression failed");
                }

            } else {
                auto [dataLen, dataLenShift] = parseVarInt<int>(dataPtr);
                dataPtr += dataLenShift;
            }

            auto [id, idShift] = parseVarInt<int>(dataPtr);

            switch (state) {
                using enum State;
                case HANDSHAKE: parseHandshakePacket<F>(id, data, compress, f); break;
                case STATUS: parseStatusPacket<F>(id, data, compress, f); break;
                case LOGIN: parseLoginPacket<F>(id, data, compress, f); break;
                case CONFIGURATION: parseConfigurationPacket<F>(id, data, compress, f); break;
                case PLAY: parsePlayPacket<F>(id, data, compress, f); break;
            }
        }

        template<typename F>
        void parseHandshakePacket(int, const std::vector<std::byte>& data, bool compress, const F& f) {
            parseUnknownPacket(data, compress, f);
        }

        template<typename F>
        void parseStatusPacket(const int id, const std::vector<std::byte>& data, bool compress, const F& f) {
            using namespace server_bound::status_step;

            switch (id) {
                case 0x00: f(ResponsePacketType::deserialize(data.data(), compress)); break;
                case 0x01: f(PongPacketType::deserialize(data.data(), compress)); break;
                default: parseUnknownPacket(data, compress, f);
            }
        }

        template<typename F>
        void parseLoginPacket(const int id, const std::vector<std::byte>& data, bool compress, const F& f) {
            using namespace server_bound::login_step;

            switch (id) {
                case 0x00: f(DisconnectPacketType::deserialize(data.data(), compress)); break;
                case 0x01: f(EncryptionRequestPacketType::deserialize(data.data(), compress)); break;
                case 0x02: f(LoginSuccessPacketType::deserialize(data.data(), compress)); break;
                case 0x03: f(CompressionPacketType::deserialize(data.data(), compress)); break;
                case 0x04: f(PluginRequestPacketType::deserialize(data.data(), compress)); break;
                default: parseUnknownPacket(data, compress, f);
            }
        }

        template<typename F>
        void parseConfigurationPacket(const int id, const std::vector<std::byte>& data, bool compress, const F& f) {
        }

        template<typename F>
        void parsePlayPacket(const int id, const std::vector<std::byte>& data, bool compress, const F& f) {
            using namespace server_bound::play_step;

            switch (id) {
                case 0x00: f(SpawnEntityPacketType::deserialize(data.data(), compress)); break;
                case 0x01: f(SpawnExperienceOrbPacketType::deserialize(data.data(), compress)); break;
                case 0x0B: f(ChangeDifficultyPacketType::deserialize(data.data(), compress)); break;
                case 0x1B: f(DisconnectPacketType::deserialize(data.data(), compress)); break;
                case 0x24: f(KeepAlivePacketType::deserialize(data.data(), compress)); break;
                case 0x26: f(SetEntityVelocityPacketType::deserialize(data.data(), compress)); break;
                case 0x29: f(LoginPacketType::deserialize(data.data(), compress)); break;
                case 0x3C: f(SpawnPlayerPacketType::deserialize(data.data(), compress)); break;
                case 0x3E: f(SpawnEntity2PacketType::deserialize(data.data(), compress)); break;
                case 0x56: f(SetPassengersPacketType::deserialize(data.data(), compress)); break;
                case 0x58: f(UpdateSectionBlocksPacketType::deserialize(data.data(), compress)); break;
                case 0x62: f(SynchronizePlayerPositionPacketType::deserialize(data.data(), compress)); break;
                case 0x66: f(UpdateRecipesPacketType::deserialize(data.data(), compress)); break;
                default: parseUnknownPacket(data, compress, f);
            }
        }

        template<typename F>
        void parseUnknownPacket(const std::vector<std::byte>& data, bool compress, const F& f) {
            f(Package<>::deserialize(data.data(), compress));
        }

    }  // namespace detail

    template<typename F>
    void parsePacket(const State state, const std::vector<std::byte>& data, bool compress, const F& f) {
#ifdef DEBUG
        Debugger parseKnownPacketDbg(&detail::parseKnownPacket<F>, [&](const auto& e) {
            detail::parseUnknownPacket(data, compress, f);
        });
        parseKnownPacketDbg(state, data, compress, f);
#else
        detail::parseKnownPacket<F>(state, data, compress, f);
#endif
    }

}  // namespace minecraft::protocol

#endif  // DEFINITION_HPP
