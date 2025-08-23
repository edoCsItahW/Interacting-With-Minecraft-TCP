// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file oldPackage.hpp 
 * @author edocsitahw 
 * @version 1.1
 * @date 2025/08/23 10:15
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef OLDPACKAGE_HPP
#define OLDPACKAGE_HPP
#pragma once


#include "../type/varNum.h"
#include "package.h"
#include <iostream>
#include <sstream>

#define ITERATOR_DEBUG_LEVEL 0

namespace minecraft::protocol {

    namespace detail {
        template<int I, isField... Ts>
        auto compressSerialize(const std::tuple<Ts...>& fields, int thresold) {
            std::vector<std::byte> uncompressedData;

            constexpr auto idBytes = VarInt<I>::serialize();
            uncompressedData.insert(uncompressedData.end(), idBytes.begin(), idBytes.end());

            std::apply(
                [&uncompressedData](const auto&... fields) {
                    (
                        [&] {
                            auto fieldBytes = fields.serialize();
                            uncompressedData.insert(uncompressedData.end(), fieldBytes.begin(), fieldBytes.end());
                        }(),
                        ...
                    );
                },
                fields
            );

            std::vector<std::byte> compressedData;
            std::size_t dataLength = 0;

            if (uncompressedData.size() > static_cast<std::size_t>(thresold)) {
                compressedData = compressData(uncompressedData);
                dataLength     = uncompressedData.size();
            } else {
                compressedData = uncompressedData;
                dataLength     = 0;
            }

            std::vector<std::byte> result;

            auto dataLengthBytes = VarInt(static_cast<int>(dataLength)).serialize();

            int packageLength       = dataLengthBytes.size() + compressedData.size();
            auto packageLengthBytes = VarInt(packageLength).serialize();

            result.insert(result.end(), packageLengthBytes.begin(), packageLengthBytes.end());
            result.insert(result.end(), dataLengthBytes.begin(), dataLengthBytes.end());
            result.insert(result.end(), compressedData.begin(), compressedData.end());

            return result;
        }

        template<int I, isField... Ts>
        auto uncompressSerialize(const std::tuple<Ts...>& fields) {
            std::vector<std::byte> result;

            constexpr auto idBytes = VarInt<I>().serialize();

            result.insert(result.end(), idBytes.begin(), idBytes.end());

            std::apply(
                [&result](const auto&... fields) {
                    (
                        [&] {
                            auto fieldBytes = fields.serialize();
                            result.insert(result.end(), fieldBytes.begin(), fieldBytes.end());
                        }(),
                        ...
                    );
                },
                fields
            );

            const auto sizeBytes = VarInt(static_cast<int>(result.size())).serialize();

            result.insert(result.begin(), sizeBytes.begin(), sizeBytes.end());

            return result;
        }

        inline auto compressDeserizlizeUnknown(const std::byte* data) {
            const std::byte* start                  = data;
            auto [packetLength, packageLengthShift] = detail::parseVarInt<int>(data);
            data += packageLengthShift;

            auto [dataLength, dataLengthShift] = detail::parseVarInt<int>(data);
            data += dataLengthShift;

            std::vector compressedData(data, data + packetLength - dataLengthShift);

            // 需要解压
            if (dataLength) {
                std::vector<std::byte> uncompressedData = decompressData(compressedData, dataLength);

                auto [id, idShift]                     = detail::parseVarInt<int>(uncompressedData.data());
                const std::byte* uncompressedDataStart = uncompressedData.data() + idShift;

                std::size_t uncompressedSize = uncompressedData.size() - idShift;
                std::vector dataVec(uncompressedDataStart, uncompressedDataStart + uncompressedSize);

                return PackageImpl<ValueArgs<>>(id, uncompressedSize, dataVec);
            }

            // 未压缩
            auto [id, idShift]                = detail::parseVarInt<int>(compressedData.data());
            const std::byte* uncompressedData = compressedData.data() + idShift;

            std::size_t uncompressedSize = compressedData.size() - idShift;
            std::vector dataVec(uncompressedData, uncompressedData + uncompressedSize);

            return PackageImpl<ValueArgs<>>(id, uncompressedSize, dataVec);
        }

        inline auto uncompressDeserizlizeUnknown(const std::byte* data) {
            auto [totalSize, shift] = detail::parseVarInt<int>(data);
            data += shift;

            auto [id, idShift] = detail::parseVarInt<int>(data);
            data += idShift;
            totalSize -= idShift;

            std::vector dataVec(data, data + totalSize);

            return PackageImpl<ValueArgs<>>(id, totalSize, dataVec);
        }

        template<typename T, isField... Ts>
        inline constexpr auto compressDeserizlizeFixed = nullptr;

        template<int I, FStrChar... Vs, isField... Ts>
        inline constexpr auto compressDeserizlizeFixed<ValueArgs<I, Vs...>, Ts...> = [](const std::byte* data) {
            const std::byte* start                 = data;
            auto [packetLength, packetLengthShift] = detail::parseVarInt<int>(data);
            data += packetLengthShift;

            auto [dataLength, dataLengthShift] = detail::parseVarInt<int>(data);
            data += dataLengthShift;

            std::vector compressedData(data, data + packetLength - dataLengthShift);

            std::vector<std::byte> uncompressedData;
            if (dataLength == 0)
                uncompressedData = compressedData;
            else
                uncompressedData = decompressData(compressedData, dataLength);

            // 解析uncompressedData中的Packet ID和Data
            const std::byte* uncompressedPtr = uncompressedData.data();
            auto [id, idShift]               = detail::parseVarInt<int>(uncompressedPtr);
            if (id != I) throw std::runtime_error("Package ID mismatch after decompression");
            uncompressedPtr += idShift;

            std::tuple<Ts...> fields;
            std::size_t offset = 0;
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                ((std::get<Is>(fields) = std::tuple_element_t<Is, std::tuple<Ts...>>::deserialize(uncompressedPtr + offset), offset += std::get<Is>(fields).size), ...);
            }(std::index_sequence_for<Ts...>{});

            if (offset != uncompressedData.size() - idShift) {
                std::cerr << "Warning: PackageImpl::deserialize: Package data mismatch after decompression. Expected " << uncompressedData.size() - idShift << " bytes, Actual: " << offset << " bytes."
                          << std::endl;
            }

            return std::apply([](auto&&... args) { return PackageImpl<ValueArgs<I, Vs...>, Ts...>(std::move(args)...); }, fields);
        };

        template<typename T, isField... Ts>
        inline constexpr auto uncompressDeserizlizeFixed = nullptr;

        template<int I, FStrChar... Vs, isField... Ts>
        inline constexpr auto uncompressDeserizlizeFixed<ValueArgs<I, Vs...>, Ts...> = [](const std::byte* data) {
            auto [totalSize, shift] = detail::parseVarInt<int>(data);
            data += shift;

            auto [id, idShift] = detail::parseVarInt<int>(data);
            totalSize -= idShift;
            data += idShift;

            std::tuple<Ts...> fields;
            std::size_t offset = 0;

            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                ((std::get<Is>(fields) = std::tuple_element_t<Is, std::tuple<Ts...>>::deserialize(data + offset), offset += std::get<Is>(fields).size), ...);
            }(std::index_sequence_for<Ts...>{});

            if (offset != totalSize)
                std::cerr << "Warning: PackageImpl::deserialize: Package data mismatch. Expected" << std::to_string(totalSize) << " bytes, Actual: " << std::to_string(offset) << " bytes."
                          << std::endl;

            return std::apply([](auto&&... args) { return PackageImpl<ValueArgs<I, Vs...>, Ts...>(std::move(args)...); }, fields);
        };

        // Unknown Package Impl

        inline PackageImpl<ValueArgs<>>::PackageImpl(int id, std::size_t size, const std::vector<std::byte>& data)
            : id_(id)
            , data_(data)
            , size_(size) {}

        inline int PackageImpl<ValueArgs<>>::id() const { return id_; }

        inline std::size_t PackageImpl<ValueArgs<>>::size() const { return size_; }

        inline std::vector<std::byte> PackageImpl<ValueArgs<>>::data() const { return data_; }

        inline auto PackageImpl<ValueArgs<>>::deserialize(const std::byte* data, bool compressed) {
            if (compressed) return compressDeserizlizeUnknown(data);
            return uncompressDeserizlizeUnknown(data);
        }

        inline std::string PackageImpl<ValueArgs<>>::toString() const {
            std::stringstream ss;

            ss << "{" << "id: " << id_ << ", ";

            ss << "data: ";

            for (auto byte : data_)
                if (const auto c = static_cast<unsigned char>(byte); std::isprint(c))
                    ss << c;
                else
                    ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";

            ss << "}";

            return ss.str();
        }

        inline std::string PackageImpl<ValueArgs<>>::toHexString() const { return minecraft::toHexString(data_); }

        // Fixed Package Impl

        template<int I, FStrChar... Vs, isField... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        constexpr PackageImpl<ValueArgs<I, Vs...>, Ts...>::PackageImpl(Ts&&... args)
            : fields_{std::forward<Ts>(args)...}
            , size_(0) {}

        template<int I, FStrChar... Vs, isField... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        std::size_t PackageImpl<ValueArgs<I, Vs...>, Ts...>::size() const {
            return size_;
        }

        template<int I, FStrChar... Vs, isField... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        std::tuple<Ts...> PackageImpl<ValueArgs<I, Vs...>, Ts...>::fields() const {
            return fields_;
        }

        template<int I, FStrChar... Vs, isField... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        template<typename>
        void PackageImpl<ValueArgs<I, Vs...>, Ts...>::onField(const char* name, /* TODO: 最适形式 */callback) const {
            return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                auto size = std::strlen(name);
                (... || (std::equal(name, name + size, std::get<Is>(names)) ? (callback(std::get<Is>(fields_)), true) : false));
            }(std::make_index_sequence<sizeof...(Ts)>{});
        }

        template<int I, FStrChar... Vs, isField... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        auto PackageImpl<ValueArgs<I, Vs...>, Ts...>::serialize(bool compressed, int thresold) const {
            if (data.empty()) data = compressed ? compressSerialize<id, Ts...>(fields_, thresold) : uncompressSerialize<id, Ts...>(fields_);

            return data;
        }

        template<int I, FStrChar... Vs, isField... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        auto PackageImpl<ValueArgs<I, Vs...>, Ts...>::deserialize(const std::byte* data, bool compressed) {
            return compressed ? compressDeserizlizeFixed<ValueArgs<I, Vs...>, Ts...>(data) : uncompressDeserizlizeFixed<ValueArgs<I, Vs...>, Ts...>(data);
        }

        template<int I, FStrChar... Vs, isField... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        std::string PackageImpl<ValueArgs<I, Vs...>, Ts...>::toString() const {
            std::stringstream ss;

            ss << "{" << "id: " << id << ", ";

            [&ss, this]<std::size_t... Is>(std::index_sequence<Is...>) {
                ((ss << names[Is] << ": " << std::get<Is>(fields_).toString() << (Is < sizeof...(Ts) - 1 ? ", " : "")), ...);
            }(std::index_sequence_for<Ts...>{});

            ss << "}";

            return ss.str();
        }

        template<int I, FStrChar... Vs, isField... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        std::string PackageImpl<ValueArgs<I, Vs...>, Ts...>::toHexString() const {
            return minecraft::toHexString(data);
        }

    }  // namespace detail

    template<isField... Ts>
    template<int I, FStrChar... Vs>
    auto TypeArgs<Ts...>::deserialize(const std::byte* data, bool compressed) {
        return detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>::deserialize(data, compressed);
    }

    template<isField... Ts>
    template<int I, FStrChar... Vs>
    auto TypeArgs<Ts...>::makePackage(detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>&& impl) {
        return Package<I, Vs...>(std::move(impl));
    }

    // Unkownd Package

    inline Package<>::Package(detail::PackageImpl<detail::ValueArgs<>> impl)
        : impl_(std::move(impl)) {}

    inline int Package<>::id() const { return impl_.id(); }

    inline std::size_t Package<>::size() const { return impl_.size(); }

    inline std::vector<std::byte> Package<>::data() const { return impl_.data(); }

    inline auto Package<>::deserialize(const std::byte* data, bool compressed) { return Package(detail::PackageImpl<detail::ValueArgs<>>::deserialize(data, compressed)); }

    inline std::string Package<>::toString() const { return impl_.toString(); }

    inline std::string Package<>::toHexString() const { return impl_.toHexString(); }

    // Fixed Package

    template<int I, FStrChar... Vs>
    template<isField... Ts>
    constexpr Package<I, Vs...>::Package(detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>&& impl) {
        auto impl_ = std::move(impl);

        serializeFunc_   = [impl_](bool compressed, int thresold) { return impl_.serialize(compressed, thresold); };
        toStringFunc_    = [impl_] { return impl_.toString(); };
        toHexStringFunc_ = [impl_] { return impl_.toHexString(); };
        getSizeFunc_     = [impl_] { return impl_.size(); };
    }

    template<int I, FStrChar... Vs>
    template<isField... Ts>
    constexpr Package<I, Vs...>::Package(Ts&&... args) {
        auto impl = detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>(std::forward<Ts>(args)...);

        serializeFunc_   = [impl](bool compressed, int thresold) { return impl.serialize(compressed, thresold); };
        toStringFunc_    = [impl] { return impl.toString(); };
        toHexStringFunc_ = [impl] { return impl.toHexString(); };
        getSizeFunc_     = [impl] { return impl.size(); };

    }

    template<int I, FStrChar... Vs>
    std::size_t Package<I, Vs...>::size() const {
        return getSizeFunc_();
    }

    template<int I, FStrChar... Vs>
    template<typename >
    void Package<I, Vs...>::onField(const char* name, /* TODO: 最适形式 */ callback) const {

    }

    template<int I, FStrChar... Vs>
    auto Package<I, Vs...>::serialize(bool compressed, int thresold) const {
        return serializeFunc_(compressed, thresold);
    }

    template<int I, FStrChar... Vs>
    template<isField... Ts>
    auto Package<I, Vs...>::deserialize(const std::byte* data, bool compressed) {
        return Package(detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>::deserialize(data, compressed));
    }

    template<int I, FStrChar... Vs>
    template<isTypeArgs T>
    auto Package<I, Vs...>::deserialize(const std::byte* data, bool compressed) {
        return T::template makePackage<I, Vs...>(T::template deserialize<I, Vs...>(data, compressed));
    }

    template<int I, FStrChar... Vs>
    std::string Package<I, Vs...>::toString() const {
        return toStringFunc_();
    }

    template<int I, FStrChar... Vs>
    std::string Package<I, Vs...>::toHexString() const {
        return toHexStringFunc_();
    }

}  // namespace minecraft::protocol


#endif //OLDPACKAGE_HPP
