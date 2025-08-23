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

#include "../../utils/fstr.h"
#include "../type/boolean.h"
#include "../type/integer.h"
#include "../type/mcuuid.h"
#include "../type/str.h"
#include "../type/varNum.h"

namespace minecraft::protocol {

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

    template<FStrChar V, isField T>
    struct FieldItem {
        static constexpr auto name = V;
        using type                 = T;
    };

    template<typename>
    struct is_field_item : std::false_type {};

    template<FStrChar V, isField T>
    struct is_field_item<FieldItem<V, T>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_field_item_v = is_field_item<T>::value;

    template<typename T>
    concept isFieldItem = is_field_item_v<T>;

    namespace detail {
        // Fixed package

        template<int I = -1, isFieldItem... Ts>
        struct PackageImpl {
        private:
            mutable std::vector<std::byte> data_;
            std::tuple<typename Ts::type...> fields_;

            std::vector<std::byte> compressSerializeImpl(int threshold) const;

            std::vector<std::byte> uncompressSerializeImpl() const;

            static PackageImpl compressDeserializeImpl(const std::byte* data);

            static PackageImpl uncompressDeserializeImpl(const std::byte* data);

        public:
            static constexpr int id = I;

            static constexpr std::size_t size = sizeof...(Ts);

            static constexpr auto names = std::array{Ts::name...};

            PackageImpl(typename Ts::type&&... args);

            template<typename F>
            void onField(F&& f, std::string key) const;

            auto serialize(bool compressed = false, int threshold = 0) const;

            static auto deserialize(const std::byte* data, bool compressed = false);

            [[nodiscard]] std::string toString() const;

            [[nodiscard]] std::string toHexString() const;
        };

        // Unknown package

        template<>
        struct PackageImpl<> {
        private:
            int id_;
            std::vector<std::byte> data_;
            std::size_t size_;

            PackageImpl(int id, std::vector<std::byte>&& data, std::size_t size);

            static PackageImpl compressDeserializeImpl(const std::byte* data);

            static PackageImpl uncompressDeserializeImpl(const std::byte* data);

        public:
            [[nodiscard]] int id() const;

            [[nodiscard]] std::vector<std::byte> data() const;

            [[nodiscard]] std::size_t size() const;

            // 未知包禁止序列化
            // std::vector<std::byte> serialize() const;

            static auto deserialize(const std::byte* data, bool compressed = false);

            [[nodiscard]] std::string toString() const;

            [[nodiscard]] std::string toHexString() const;
        };

        PackageImpl(int, std::vector<std::byte>&&, std::size_t) -> PackageImpl<>;

        template<typename>
        struct is_package_impl : std::false_type {};

        template<int I, isFieldItem... Ts>
        struct is_package_impl<PackageImpl<I, Ts...>> : std::true_type {};

        template<typename T>
        inline constexpr bool is_package_impl_v = is_package_impl<T>::value;

        template<typename T>
        concept isPackageImpl = is_package_impl_v<T>;
    }  // namespace detail

    template<detail::isPackageImpl T>
    struct Package;

    template<int I, FStrChar... Vs, isField... Ts>
    struct Package<detail::PackageImpl<I, FieldItem<Vs, Ts>...>> {
    private:
        using ImplType = detail::PackageImpl<I, FieldItem<Vs, Ts>...>;

    public:
        Package(Ts&&... args);
    };

    template<typename>
    struct is_package : std::false_type {};

    template<int I, FStrChar... Vs, isField... Ts>
    struct is_package<Package<detail::PackageImpl<I, FieldItem<Vs, Ts>...>>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_package_v = is_package<T>::value;

    template<typename T>
    concept isPackage = is_package_v<T>;

}  // namespace minecraft::protocol

#include "PackageImpl.hpp"

#endif  // PACKAGE_H
