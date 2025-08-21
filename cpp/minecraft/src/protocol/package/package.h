// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file package.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/18 20:27
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef PACKAGE_H
#define PACKAGE_H
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

        template<typename T>
        concept is_builtin_field = is_boolean_field_v<T> || is_integer_field_v<T> || is_uuid_field_v<T> || is_string_field_v<T> || is_var_num_field_v<T>;

        template<typename T>
        concept is_custom_field = requires {
            T::deserializer;

            requires std::is_invocable_v<decltype(T::deserializer), const std::byte*>;

            requires requires(T t) {
                { t.serialize() };
                requires is_byte_array_v<std::remove_cvref_t<decltype(t.serialize())>> || std::is_same_v<std::remove_cvref_t<decltype(t.serialize())>, std::vector<std::byte>>;
            } || requires {
                { T::serialize() };
                requires is_byte_array_v<std::remove_cvref_t<decltype(T::serialize())>>;
            };
        };

        template<typename T>
        concept is_field = is_builtin_field<T> || is_custom_field<T>;

        template<auto...>
        struct ValueArgs {};

        template<typename, is_field...>
        struct PackageImpl;

        template<>
        struct PackageImpl<ValueArgs<>> {
        private:
            int id_;
            std::vector<std::byte> data_;
            std::size_t size_;

            PackageImpl(int id, std::size_t size, const std::vector<std::byte>& data);

        public:
            [[nodiscard]] int id() const;

            [[nodiscard]] std::size_t size() const;

            [[nodiscard]] std::vector<std::byte> data() const;

            ///< 未知的包禁止序列化
            // std::vector<std::byte> serialize() const;

            static auto deserialize(const std::byte* data);

            [[nodiscard]] std::string toString() const;

            [[nodiscard]] std::string toHexString() const;
        };

        template<int I, FStrChar... Vs, is_field... Ts>
            requires(sizeof...(Ts) == sizeof...(Vs))
        struct PackageImpl<ValueArgs<I, Vs...>, Ts...> {
        private:
            std::tuple<Ts...> fields;
            mutable std::vector<std::byte> data;
            mutable std::size_t size_;

        public:
            static constexpr auto id = I;

            static constexpr std::array<const char*, sizeof...(Vs)> names = std::array{Vs.data.data()...};

            constexpr PackageImpl(Ts&&... args);

            [[nodiscard]] std::size_t size() const;

            auto serialize() const;

            static auto deserialize(const std::byte* data);

            [[nodiscard]] std::string toString() const;

            [[nodiscard]] std::string toHexString() const;
        };


    }  // namespace detail

    template<detail::is_field...>
    struct TypeArgs;

    template<typename>
    struct is_type_args : std::false_type {};

    template<detail::is_field... Ts>
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

        std::size_t size() const;

        [[nodiscard]] std::vector<std::byte> data() const;

        [[nodiscard]] static auto deserialize(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<int I, FStrChar... Vs>
    struct Package<I, Vs...> {
    private:
        std::function<std::vector<std::byte>()> serializeFunc_;
        std::function<std::string()> toStringFunc_;
        std::function<std::string()> toHexStringFunc_;
        std::function<std::size_t()> getSizeFunc_;

    public:
        static constexpr auto id = I;

        static constexpr std::array<const char*, sizeof...(Vs)> names = std::array{Vs.data.data()...};

        template<detail::is_field... Ts>
        constexpr Package(detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>&& impl);

        template<detail::is_field... Ts>
        constexpr Package(Ts&&... args);

        [[nodiscard]] std::size_t size() const;

        auto serialize() const;

        template<detail::is_field... Ts>
        static auto deserialize(const std::byte* data);

        template<isTypeArgs T>
        static auto deserialize(const std::byte* data);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    template<detail::is_field... Ts>
    struct TypeArgs {
        template<int I, FStrChar... Vs>
        static auto deserialize(const std::byte* data);

        template<int I, FStrChar... Vs>
        static auto makePackage(detail::PackageImpl<detail::ValueArgs<I, Vs...>, Ts...>&& impl);
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

#include "package.hpp"

#endif  // PACKAGE_H
