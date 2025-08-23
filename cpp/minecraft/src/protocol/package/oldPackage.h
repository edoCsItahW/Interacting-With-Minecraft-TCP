// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file oldPackage.h 
 * @author edocsitahw 
 * @version 1.1
 * @date 2025/08/23 10:14
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef OLDPACKAGE_H
#define OLDPACKAGE_H
#pragma once

#include "../type/boolean.h"
#include "../type/integer.h"
#include "../type/mcuuid.h"
#include "../type/str.h"
#include "../type/varNum.h"
#include <functional>
#include <string>
#include <vector>

namespace minecraft::protocol {
    template<auto...>
    struct Package;

    namespace detail {
        template<typename>
        struct is_byte_array : std::false_type {};

        template<std::size_t N>
        struct is_byte_array<std::array<std::byte, N>> : std::true_type {};

        template<typename T>
        inline constexpr bool is_byte_array_v = is_byte_array<T>::value;
    }  // namespace detail

    template<typename T>
    concept is_builtin_field = is_boolean_field_v<T> || is_integer_field_v<T> || is_uuid_field_v<T> || is_string_field_v<T> || is_var_num_field_v<T>;

    template<typename T>
    concept is_custom_field = requires {
        T::deserializer;

        requires std::is_invocable_v<decltype(T::deserializer), const std::byte*>;

        requires requires(T t) {
            { t.serialize() };
            requires detail::is_byte_array_v<std::remove_cvref_t<decltype(t.serialize())>> || std::is_same_v<std::remove_cvref_t<decltype(t.serialize())>, std::vector<std::byte>>;
        } || requires {
            { T::serialize() };
            requires detail::is_byte_array_v<std::remove_cvref_t<decltype(T::serialize())>>;
        };
    };

    template<typename T>
    concept isField = is_builtin_field<T> || is_custom_field<T>;

    namespace detail {

        template<int I, isField... Ts>
        auto compressSerialize(const std::tuple<Ts...>& fields, int thresold);

        template<int I, isField... Ts>
        auto uncompressSerialize(const std::tuple<Ts...>& fields);

        auto compressDeserizlizeUnkown(const std::byte* data);

        auto uncompressDeserizlizeUnkown(const std::byte* data);

        template<auto...>
        struct ValueArgs {};

        template<typename, isField...>
        struct PackageImpl;

        template<>
        struct PackageImpl<ValueArgs<>> {
        private:
            int id_;
            std::vector<std::byte> data_;
            std::size_t size_;

            PackageImpl(int id, std::size_t size, const std::vector<std::byte>& data);

            friend auto compressDeserizlizeUnknown(const std::byte* data);
            friend auto uncompressDeserizlizeUnknown(const std::byte* data);

        public:
            PackageImpl(const PackageImpl& other) = default;

            PackageImpl(PackageImpl&& other) noexcept = default;

            PackageImpl& operator=(const PackageImpl& other) = default;

            PackageImpl& operator=(PackageImpl&& other) noexcept = default;

            ~PackageImpl() = default;

            [[nodiscard]] int id() const;

            [[nodiscard]] std::size_t size() const;

            [[nodiscard]] std::vector<std::byte> data() const;

            ///< 未知的包禁止序列化
            // std::vector<std::byte> serialize() const;

            static auto deserialize(const std::byte* data, bool compressed);

            [[nodiscard]] std::string toString() const;

            [[nodiscard]] std::string toHexString() const;
        };

        template<typename>
        struct is_package_impl : std::false_type {};

        template<>
        struct is_package_impl<PackageImpl<ValueArgs<>>> : std::true_type {};

        template<int I, FStrChar... Vs, isField... Ts>
        struct is_package_impl<PackageImpl<ValueArgs<I, Vs...>, Ts...>> : std::true_type {};

        template<typename T>
        inline constexpr bool is_package_impl_v = is_package_impl<T>::value;

        template<typename T>
        concept isPackageImpl = is_package_impl_v<T>;

        template<int I, FStrChar... Vs, isField... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        struct PackageImpl<ValueArgs<I, Vs...>, Ts...> {
        private:
            std::tuple<Ts...> fields_;
            mutable std::vector<std::byte> data;
            mutable std::size_t size_;

        public:
            static constexpr auto id = I;

            static constexpr std::array<const char*, sizeof...(Vs)> names = std::array{Vs.data.data()...};

            constexpr PackageImpl(Ts&&... args);

            constexpr PackageImpl(const PackageImpl& other) = default;

            constexpr PackageImpl(PackageImpl&& other) noexcept = default;

            constexpr PackageImpl& operator=(const PackageImpl& other) = default;

            constexpr PackageImpl& operator=(PackageImpl&& other) noexcept = default;

            [[nodiscard]] std::size_t size() const;

            [[nodiscard]] std::tuple<Ts...> fields() const;

            template<typename >
            void onField(const char* name, /* TODO: 最适形式 */ callback) const;

            auto serialize(bool compressed, int thresold) const;

            static auto deserialize(const std::byte* data, bool compressed = false);

            [[nodiscard]] std::string toString() const;

            [[nodiscard]] std::string toHexString() const;
        };

    }  // namespace detail

    template<isField...>
    struct TypeArgs;

    template<isField... Ts>
    struct TypeArgs {
        template<int I, FStrChar... Vs>
        static auto deserialize(const std::byte* data, bool compressed = false);

        template<int I, FStrChar... Vs>
        static auto makePackage(detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>&& impl);
    };

    template<typename>
    struct is_type_args : std::false_type {};

    template<isField... Ts>
    struct is_type_args<TypeArgs<Ts...>> : std::true_type {};

    template<typename T>
    concept isTypeArgs = is_type_args<T>::value;

    template<>
    struct Package<> {
    private:
        detail::PackageImpl<detail::ValueArgs<>> impl_;

        Package(detail::PackageImpl<detail::ValueArgs<>> impl);

    public:
        [[nodiscard]] int id() const;

        [[nodiscard]] std::size_t size() const;

        [[nodiscard]] std::vector<std::byte> data() const;

        [[nodiscard]] static auto deserialize(const std::byte* data, bool compressed = false);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<int I, FStrChar... Vs>
    struct Package<I, Vs...> {
    private:
        std::function<std::vector<std::byte>(bool, int)> serializeFunc_;
        std::function<std::string()> toStringFunc_;
        std::function<std::string()> toHexStringFunc_;
        std::function<std::size_t()> getSizeFunc_;

        std::function<void(const char*,)>

        template<isField... Ts>
        constexpr Package(detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>&& impl);

        template<isField... Ts>
        friend struct TypeArgs;  // 使TypeArgs可以访问Package的private构造函数

    public:
        static constexpr auto id = I;

        static constexpr std::array<const char*, sizeof...(Vs)> names = std::array{Vs.data.data()...};

        template<isField... Ts>
        constexpr Package(Ts&&... args);

        [[nodiscard]] std::size_t size() const;

        template<typename >
        void onField(const char* name, /* TODO: 最适形式 */ callback) const;

        auto serialize(bool compressed = false, int thresold = 0) const;

        template<isField... Ts>
        static auto deserialize(const std::byte* data, bool compressed = false);

        template<isTypeArgs T>
        static auto deserialize(const std::byte* data, bool compressed = false);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<typename>
    struct is_rt_package : std::false_type {};

    template<>
    struct is_rt_package<Package<>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_rt_package_v = is_rt_package<T>::value;

    template<typename T>
    concept isRTPackage = is_rt_package_v<T>;

    template<typename>
    struct is_ct_package : std::false_type {};

    template<int I, FStrChar... Vs>
    struct is_ct_package<Package<I, Vs...>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_ct_package_v = is_ct_package<T>::value;

    template<typename T>
    concept isCTPackage = is_ct_package_v<T>;

    template<typename T>
    concept isPackage = isRTPackage<T> || isCTPackage<T>;

}  // namespace minecraft::protocol

#include "oldPackage.hpp"

#endif //OLDPACKAGE_H
