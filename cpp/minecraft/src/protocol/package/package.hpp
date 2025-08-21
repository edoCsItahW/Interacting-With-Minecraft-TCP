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

#include "../type/varNum.h"
#include "package.h"
#include <iostream>
#include <sstream>

#define ITERATOR_DEBUG_LEVEL 0

namespace minecraft::protocol {

    namespace detail {
        inline PackageImpl<ValueArgs<>>::PackageImpl(int id, std::size_t size, const std::vector<std::byte>& data)
            : id_(id)
            , data_(data)
            , size_(size) {}

        inline int PackageImpl<ValueArgs<>>::id() const { return id_; }

        inline std::size_t PackageImpl<ValueArgs<>>::size() const { return size_; }

        inline std::vector<std::byte> PackageImpl<ValueArgs<>>::data() const { return data_; }

        inline auto PackageImpl<ValueArgs<>>::deserialize(const std::byte* data) {
            auto [totalSize, shift] = detail::parseVarInt<int>(data);
            data += shift;

            auto [id, idShift] = detail::parseVarInt<int>(data);
            data += idShift;
            totalSize -= idShift;

            std::vector dataVec(data, data + totalSize);

            return PackageImpl(id, totalSize, dataVec);
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

        template<int I, FStrChar... Vs, is_field... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        constexpr PackageImpl<ValueArgs<I, Vs...>, Ts...>::PackageImpl(Ts&&... args)
            : fields{std::forward<Ts>(args)...}
            , size_(0) {}

        template<int I, FStrChar... Vs, is_field... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        std::size_t PackageImpl<ValueArgs<I, Vs...>, Ts...>::size() const {
            return size_;
        }

        template<int I, FStrChar... Vs, is_field... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        auto PackageImpl<ValueArgs<I, Vs...>, Ts...>::serialize() const {
            if (data.empty()) {
                constexpr auto idBytes = VarInt<id>().serialize();

                data.insert(data.end(), idBytes.begin(), idBytes.end());

                std::apply(
                    [this](const auto&... fields) {
                        (
                            [&] {
                                auto fieldBytes = fields.serialize();
                                data.insert(data.end(), fieldBytes.begin(), fieldBytes.end());
                            }(),
                            ...
                        );
                    },
                    fields
                );

                const auto sizeBytes = VarInt(data.size()).serialize();

                data.insert(data.begin(), sizeBytes.begin(), sizeBytes.end());
            }

            return data;
        }

        template<int I, FStrChar... Vs, is_field... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        auto PackageImpl<ValueArgs<I, Vs...>, Ts...>::deserialize(const std::byte* data) {
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

            return std::apply([](auto&&... args) { return PackageImpl(std::move(args)...); }, fields);
        }

        template<int I, FStrChar... Vs, is_field... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        std::string PackageImpl<ValueArgs<I, Vs...>, Ts...>::toString() const {
            std::stringstream ss;

            ss << "{" << "id: " << id << ", ";

            [&ss, this]<std::size_t... Is>(std::index_sequence<Is...>) {
                ((ss << names[Is] << ": " << std::get<Is>(fields).toString() << (Is < sizeof...(Ts) - 1 ? ", " : "")), ...);
            }(std::index_sequence_for<Ts...>{});

            ss << "}";

            return ss.str();
        }

        template<int I, FStrChar... Vs, is_field... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        std::string PackageImpl<ValueArgs<I, Vs...>, Ts...>::toHexString() const {
            return minecraft::toHexString(serialize());
        }

    }  // namespace detail

    template<detail::is_field... Ts>
    template<int I, FStrChar... Vs>
    auto TypeArgs<Ts...>::deserialize(const std::byte* data) {
        return detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>::deserialize(data);
    }

    template<detail::is_field... Ts>
    template<int I, FStrChar... Vs>
    auto TypeArgs<Ts...>::makePackage(detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>&& impl) {
        return Package<I, Vs...>(std::move(impl));
    }

    inline Package<>::Package(detail::PackageImpl<detail::ValueArgs<>> impl)
        : impl_(std::move(impl)) {}

    inline int Package<>::id() const { return impl_.id(); }

    inline std::size_t Package<>::size() const { return impl_.size(); }

    inline std::vector<std::byte> Package<>::data() const { return impl_.data(); }

    inline auto Package<>::deserialize(const std::byte* data) { return Package(detail::PackageImpl<detail::ValueArgs<>>::deserialize(data)); }

    inline std::string Package<>::toString() const { return impl_.toString(); }

    inline std::string Package<>::toHexString() const { return impl_.toHexString(); }

    template<int I, FStrChar... Vs>
    template<detail::is_field... Ts>
    constexpr Package<I, Vs...>::Package(detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>&& impl) {
        auto impl_ = std::move(impl);

        serializeFunc_   = [impl_] { return impl_.serialize(); };
        toStringFunc_    = [impl_] { return impl_.toString(); };
        toHexStringFunc_ = [impl_] { return impl_.toHexString(); };
        getSizeFunc_     = [impl_] { return impl_.size(); };
    }

    template<int I, FStrChar... Vs>
    template<detail::is_field... Ts>
    constexpr Package<I, Vs...>::Package(Ts&&... args) {
        auto impl = detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>(std::forward<Ts>(args)...);

        serializeFunc_   = [impl] { return impl.serialize(); };
        toStringFunc_    = [impl] { return impl.toString(); };
        toHexStringFunc_ = [impl] { return impl.toHexString(); };
        getSizeFunc_     = [impl] { return impl.size(); };
    }

    template<int I, FStrChar... Vs>
    std::size_t Package<I, Vs...>::size() const {
        return getSizeFunc_();
    }

    template<int I, FStrChar... Vs>
    auto Package<I, Vs...>::serialize() const {
        return serializeFunc_();
    }

    template<int I, FStrChar... Vs>
    template<detail::is_field... Ts>
    auto Package<I, Vs...>::deserialize(const std::byte* data) {
        return Package(detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>::deserialize(data));
    }

    template<int I, FStrChar... Vs>
    template<isTypeArgs T>
    auto Package<I, Vs...>::deserialize(const std::byte* data) {
        return T::template makePackage<I, Vs...>(T::template deserialize<I, Vs...>(data));
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

#endif  // PACKAGE_HPP
