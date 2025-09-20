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
#include "../../utils/nullable.h"
#include "../type/angle.h"
#include "../type/boolean.h"
#include "../type/compoundArray.h"
#include "../type/double.h"
#include "../type/float.h"
#include "../type/identifier.h"
#include "../type/integer.h"
#include "../type/mcOption.h"
#include "../type/mcuuid.h"
#include "../type/position.h"
#include "../type/prefixedArray.h"
#include "../type/prefixedOption.h"
#include "../type/str.h"
#include "../type/varNum.h"
#include <optional>

namespace minecraft::protocol {

    namespace detail {
        template<typename>
        struct is_byte_array : std::false_type {};

        template<std::size_t N>
        struct is_byte_array<std::array<std::byte, N>> : std::true_type {};

        template<>
        struct is_byte_array<std::vector<std::byte>> : std::true_type {};

        template<typename T>
        inline constexpr bool is_byte_array_v = is_byte_array<T>::value;

        template<typename T>
        concept is_builtin_field =
            is_boolean_field<T> || is_integer_field<T> || is_uuid_field<T> || is_string_field<T> || is_var_num_field<T> || is_array_field<T> || is_float_field<T> || is_double_field<T>
            || is_angle_field<T> || is_position_field<T> || is_prefixed_array_field<T> || is_identifier_field<T> || is_option_field<T> || is_prefixed_option_field<T> || is_compound_array_field<T>;

        template<typename T>
        concept is_custom_field = requires {
            T::deserialize;

            requires std::is_invocable_v<decltype(T::deserialize), const std::byte*>;

            requires requires(T t) {
                { t.serialize() };
                requires detail::is_byte_array_v<std::remove_cvref_t<typename T::serializeType>>;
            } || requires {
                { T::serialize() };
                requires detail::is_byte_array_v<std::remove_cvref_t<typename T::serializeType>>;
            };
        };


        template<typename T>
        concept is_nullable_fstr = is_nullable<T> && (is_fstr_char<typename T::type> || std::is_same_v<typename T::type, void>);

    }  // namespace detail

    template<typename T>
    concept is_field = detail::is_builtin_field<T> || detail::is_custom_field<T>;

    template<FStrChar V, is_field T, detail::is_nullable_fstr auto D = Null>
    struct FieldItem {
        static constexpr auto name = V;
        using type                 = T;
        static constexpr auto dep  = D;
    };

    template<typename>
    struct isFieldItem : std::false_type {};

    template<FStrChar V, is_field T, detail::is_nullable_fstr auto D>
    struct isFieldItem<FieldItem<V, T, D>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_field_item_v = isFieldItem<T>::value;

    template<typename T>
    concept is_field_item = is_field_item_v<T>;

    namespace detail {
        template<FStrChar V, std::size_t I, is_field_item T, is_field_item... Ts>
        struct IndexOfNameImpl {
            static constexpr int value = V == T::name ? I : IndexOfNameImpl<V, I + 1, Ts...>::value;
        };

        template<FStrChar V, std::size_t I, is_field_item T>
        struct IndexOfNameImpl<V, I, T> {
            static constexpr int value = V == T::name ? I : -1;
        };

    }  // namespace detail

    template<FStrChar V, is_field_item... Ts>
    struct indexOfName {
        static constexpr int value = sizeof...(Ts) == 0 ? -1 : detail::IndexOfNameImpl<V, 0, Ts...>::value;
    };

    template<FStrChar V, is_field_item... Ts>
    inline constexpr auto indexOfName_v = indexOfName<V, Ts...>::value;

    namespace detail {

        template<typename T, typename F>
        constexpr decltype(auto) forEach(T&& tuple, F&& f);

        template<is_field_item... Ts>
        struct FieldMap {
        private:
            static constexpr auto keys = std::tuple{Ts::name...};

            using TupleType = std::tuple<typename Ts::type...>;

            TupleType* tuplePtr;

        public:
            FieldMap(TupleType& tuple);

            template<FStrChar V>
            auto get() const;

            template<std::size_t I>
            auto get() const;

            template<typename F>
            void on(const std::string& key, F&& f) const;

            template<FStrChar V>
            [[nodiscard]] constexpr bool has() const;

            template<std::size_t N>
            constexpr bool has(FStrChar<N> v) const;

            template<typename F>
            void forEach(F&& f);
        };

    }  // namespace detail

    // Fixed package

    template<int I = -1, is_field_item... Ts>
    struct Package {
    private:
        mutable std::vector<std::byte> data_;

        std::tuple<typename Ts::type...> fields_;

        detail::FieldMap<Ts...> fieldMap_;

        std::vector<std::byte> compressSerializeImpl(int threshold) const;

        std::vector<std::byte> uncompressSerializeImpl() const;

        static Package compressDeserializeImpl(const std::byte* data);

        static Package uncompressDeserializeImpl(const std::byte* data);

        Package(std::tuple<typename Ts::type...> fields);

        template<std::size_t Idx>
        using typeOf = std::tuple_element_t<Idx, std::tuple<typename Ts::type...>>;

    public:
        static constexpr int id = I;

        static constexpr std::size_t size = sizeof...(Ts);

        static constexpr auto names = std::tuple{Ts::name...};

        Package(typename Ts::type&&... args);

        template<typename F>
        void onField(std::string key, F&& f) const;

        template<typename F>
        void forEachField(F&& f);

        template<typename F>
        static void forEachType(F&& f);

        template<FStrChar V>
        auto get() const;

        auto serialize(bool compressed = false, int threshold = 0) const;

        static auto deserialize(const std::byte* data, bool compressed = false);

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toHexString() const;
    };

    // Unknown package

    template<>
    struct Package<> {
    private:
        int id_;
        std::vector<std::byte> data_;
        std::size_t size_;

        Package(int id, std::vector<std::byte>&& data, std::size_t size);

        static Package compressDeserializeImpl(const std::byte* data);

        static Package uncompressDeserializeImpl(const std::byte* data);

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

    Package(int, std::vector<std::byte>&&, std::size_t) -> Package<>;

    template<typename>
    struct isPackage : std::false_type {};

    template<int I, is_field_item... Ts>
    struct isPackage<Package<I, Ts...>> : std::true_type {};

    template<typename T>
    inline constexpr bool isPackage_v = isPackage<T>::value;

    template<typename T>
    concept is_package = isPackage_v<T>;

}  // namespace minecraft::protocol

namespace std {
    template<int I, minecraft::protocol::is_field_item... Ts>
    struct tuple_size<minecraft::protocol::Package<I, Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

    template<std::size_t Idx, int I, minecraft::protocol::is_field_item... Ts>
    struct tuple_element<Idx, minecraft::protocol::Package<I, Ts...>> {
        static_assert(Idx < sizeof...(Ts), "Index out of bounds");

        using type = std::tuple_element_t<Idx, std::tuple<typename Ts::type...>>;
    };

}  // namespace std

#include "package.hpp"

#endif  // PACKAGE_H
