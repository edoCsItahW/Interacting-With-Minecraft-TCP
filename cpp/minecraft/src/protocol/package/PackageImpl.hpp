// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file package.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 21:45
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef PACKAGE_HPP
#define PACKAGE_HPP
#pragma once

#include <iostream>
#include <sstream>

namespace minecraft::protocol {

    namespace detail {

        inline PackageImpl<> PackageImpl<>::compressDeserializeImpl(const std::byte* data) {
            auto [packetLen, packetLenShift] = detail::parseVarInt<int>(data);
            data += packetLenShift;

            auto [dataLen, dataLenShift] = detail::parseVarInt<int>(data);
            data += dataLenShift;

            std::vector cData(data, data + packetLen - dataLenShift);

            if (dataLen) {
                std::vector<std::byte> uData = decompressData(cData, dataLen);

                auto [id, idShift] = detail::parseVarInt<int>(uData.data());

                const std::byte* ptr = uData.data() + idShift;

                std::size_t resSize = uData.size() - idShift;

                std::vector resData(ptr, ptr + resSize);

                return {id, std::move(resData), resSize};
            }

            auto [id, idShift] = detail::parseVarInt<int>(cData.data());

            const std::byte* ptr = cData.data() + idShift;

            std::size_t resSize = cData.size() - idShift;

            std::vector resData(ptr, ptr + resSize);

            return {id, std::move(resData), resSize};
        }

        inline PackageImpl<> PackageImpl<>::uncompressDeserializeImpl(const std::byte* data) {
            auto [len, lenShift] = detail::parseVarInt<int>(data);
            data += lenShift;

            auto [id, idShift] = detail::parseVarInt<int>(data);
            data += idShift;
            len -= idShift;

            std::vector result(data, data + len);

            return {id, std::move(result), static_cast<std::size_t>(len)};
        }

        inline PackageImpl<>::PackageImpl(const int id, std::vector<std::byte>&& data, const std::size_t size)
            : id_(id)
            , data_(std::move(data))
            , size_(size) {}

        inline int PackageImpl<>::id() const { return id_; }

        inline std::vector<std::byte> PackageImpl<>::data() const { return data_; }

        inline std::size_t PackageImpl<>::size() const { return size_; }

        inline auto PackageImpl<>::deserialize(const std::byte* data, bool compressed) { return compressed ? compressDeserializeImpl(data) : uncompressDeserializeImpl(data); }

        inline std::string PackageImpl<>::toString() const {
            std::stringstream ss;

            ss << "{ id: " << id_ << ", data: ";

            for (auto b : data_)
                if (const auto c = static_cast<char>(b); std::isprint(c))
                    ss << c;
                else
                    ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b) << " ";

            ss << "}";

            return ss.str();
        }

        inline std::string PackageImpl<>::toHexString() const { return minecraft::toHexString(data_); }

        // Fixed package

        template<int I, isFieldItem... Ts>
        std::vector<std::byte> PackageImpl<I, Ts...>::compressSerializeImpl(int threshold) const {
            std::vector<std::byte> data;

            constexpr auto idBytes = VarInt<I>::serialize();
            data.insert_range(data.end(), idBytes);

            std::apply(
                [&](const auto&... fs) {
                    (
                        [&] {
                            auto fieldBytes = fs.serialize();
                            data.insert_range(data.end(), fieldBytes);
                        }(),
                        ...
                    );
                },
                fields_
            );

            std::vector<std::byte> cData;
            std::size_t cSize = 0;

            // 大于阈值压缩
            if (data.size() > threshold) {
                cData = compressData(data);
                cSize = cData.size();
            }

            // 小于阈值不压缩
            else {
                cData = std::move(data);
                cSize = 0;
            }

            std::vector<std::byte> result;

            auto sizeBytes      = VarInt(static_cast<int>(cSize)).serialize();
            auto packetLenBytes = VarInt(static_cast<int>(sizeBytes.size() + cData.size())).serialize();

            result.insert_range(result.end(), packetLenBytes);
            result.insert_range(result.end(), sizeBytes);
            result.insert_range(result.end(), cData);

            return result;
        }

        template<int I, isFieldItem... Ts>
        std::vector<std::byte> PackageImpl<I, Ts...>::uncompressSerializeImpl() const {
            std::vector<std::byte> result;

            constexpr auto idBytes = VarInt<I>::serialize();

            result.insert_range(result.end(), idBytes);

            std::apply(
                [&](const auto&... fs) {
                    (
                        [&] {
                            auto fieldBytes = fs.serialize();
                            result.insert_range(result.end(), fieldBytes);
                        }(),
                        ...
                    );
                },
                fields_
            );

            const auto sizeBytes = VarInt(static_cast<int>(result.size())).serialize();

            result.insert_range(result.end(), sizeBytes);

            return result;
        }

        template<int I, isFieldItem... Ts>
        PackageImpl<I, Ts...> PackageImpl<I, Ts...>::compressDeserializeImpl(const std::byte* data) {
            auto [packetLen, packetLenShift] = detail::parseVarInt<int>(data);
            data += packetLenShift;

            auto [dataLen, dataLenShift] = detail::parseVarInt<int>(data);
            data += dataLenShift;

            std::vector cData(data, data + packetLen - dataLenShift);

            std::vector<std::byte> result;
            if (dataLen == 0)
                result = std::move(cData);
            else
                result = decompressData(cData, dataLen);

            const std::byte* ptr = result.data();

            auto [id, idShift] = detail::parseVarInt<int>(ptr);
            ptr += idShift;

            if (id != I) throw std::runtime_error("PackageImpl ID mismatch after decompression.");

            std::tuple<typename Ts::type...> fields;

            std::size_t offset = 0;

            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                ((std::get<Is>(fields) = std::tuple_element_t<Is, std::tuple<typename Ts::type...>>::deserializer(ptr + offset), offset += std::get<Is>(fields).size), ...);
            }(std::make_index_sequence<size>());

            if (offset != result.size() - idShift)
                std::cerr << "Warning: PackageImpl::deserialize: PackageImpl data mismatch after decompression. Expected " << result.size() - idShift << " bytes, Actual: " << offset << " bytes."
                          << std::endl;

            return std::apply([]<typename... T>(T&&... args) { return Package(std::forward<T>(args)...); }, fields);
        }

        template<int I, isFieldItem... Ts>
        PackageImpl<I, Ts...> PackageImpl<I, Ts...>::uncompressDeserializeImpl(const std::byte* data) {
            auto [len, lenShift] = detail::parseVarInt<int>(data);
            data += lenShift;

            auto [id, idShift] = detail::parseVarInt<int>(data);
            data += idShift;
            len -= idShift;

            std::tuple<typename Ts::type...> fields;

            std::size_t offset = 0;

            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                ((std::get<Is>(fields) = std::tuple_element_t<Is, std::tuple<typename Ts::type...>>::deserializer(data + offset), offset += std::get<Is>(fields).size), ...);
            }(std::make_index_sequence<size>());

            if (offset != len) std::cerr << "Warning: PackageImpl::deserialize: PackageImpl data mismatch. Expected " << len << " bytes, Actual: " << offset << " bytes." << std::endl;

            return std::apply([]<typename... T>(T&&... args) { return Package(std::forward<T>(args)...); }, fields);
        }

        template<int I, isFieldItem... Ts>
        PackageImpl<I, Ts...>::PackageImpl(typename Ts::type&&... args)
            : fields_{std::forward<typename Ts::type>(args)...} {}

        template<int I, isFieldItem... Ts>
        template<typename F>
        void PackageImpl<I, Ts...>::onField(F&& f, std::string key) const {
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                (... || (std::get<Is>(names).equals(key.data(), key.size()) ? (f(std::get<Is>(fields_)), true) : false));
            }(std::make_index_sequence<size>{});
        }

        template<int I, isFieldItem... Ts>
        auto PackageImpl<I, Ts...>::serialize(bool compressed, int threshold) const {
            if (data_.empty()) data_ = compressed ? compressSerializeImpl(threshold) : uncompressSerializeImpl();

            return data_;
        }

        template<int I, isFieldItem... Ts>
        auto PackageImpl<I, Ts...>::deserialize(const std::byte* data, bool compressed) {
            return compressed ? compressDeserializeImpl(data) : uncompressDeserializeImpl(data);
        }

        template<int I, isFieldItem... Ts>
        std::string PackageImpl<I, Ts...>::toString() const {
            std::stringstream ss;

            ss << "{ id: " << id << ", ";

            [&ss, this]<std::size_t... Is>(std::index_sequence<Is...>) {
                (..., (ss << std::get<Is>(names).data.data() << ": " << std::get<Is>(fields_).toString() << (Is < size - 1 ? ", " : "")));
            }(std::index_sequence<size>{});

            ss << "}";

            return ss.str();
        }

        template<int I, isFieldItem... Ts>
        std::string PackageImpl<I, Ts...>::toHexString() const {
            return minecraft::toHexString(data_);
        }
    }  // namespace detail

    // Unkown package


}  // namespace minecraft::protocol

#endif  // PACKAGE_HPP
