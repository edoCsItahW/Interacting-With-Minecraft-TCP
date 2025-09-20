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

#include "package.h"
#include <iostream>
#include <sstream>

#ifdef DEBUG
    #include "../../utils/debugger.h"
#endif

/**
 * @if zh
 *
 * @page 压缩与解压缩
 *
 * @section 压缩
 *
 * 是否压缩取决于服务器在登录阶段发送的ID为3的`SetCompression`包中@c threshold 的值，如果该值大于0，则表示启用压缩，否则表示不启用压缩。
 *
 * 压缩有两种情况：
 *
 * @subsection 1. 长度小于阈值
 *
 * 这种情况采用小数据包的压缩方式，格式为
 *
 * | 字段顺序 | 字段名称  | 字段类型  | 说明  |
 * | :----- | :-----   | :-----   | :--- |
 * | 1      | 数据包长度 | VarInt | 长度（数据长度 + 数据包ID + 数据）  |
 * | 2      | 数据长度  | VarInt | 必须为0 |
 * | 3      | 数据包ID | VarInt | 未压缩的数据包ID |
 * | 4      | 数据     | Byte Array | 未压缩的数据 |
 *
 * > `数据长度`字段被设置为0，这是一个标志，表示@a 这个包虽然处于压缩模式，但没有压缩
 *
 * @subsection 2. 长度大于等于阈值
 *
 * 正在的使用zlib进行数据压缩，格式为
 *
 * | 字段顺序 | 字段名称  | 字段类型  | 说明  |
 * | :----- | :-----   | :-----   | :--- |
 * | 1      | 数据包长度 | VarInt | 长度（数据长度 + 压缩后的数据） |
 * | 2      | 数据长度  | VarInt | 未压缩（数据包ID + 数据）的字节数 |
 * | 3      | 数据包ID + 数据 | Byte Array | 这是一个整体，是经过 zlib 压缩后的（数据包 ID + 数据） |
 *
 * @else
 *
 * @endif
 *
 * */


namespace minecraft::protocol {

    namespace detail {

        template<typename T, typename F>
        constexpr decltype(auto) forEach(T&& tuple, F&& f) {
            return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                (f(std::get<Is>(tuple)), ...);
                return std::forward<T>(tuple);
            }(std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<T>>>{});
        }

        template<is_field_item... Ts>
        FieldMap<Ts...>::FieldMap(TupleType& tuple)
            : tuplePtr(&tuple) {}

        template<is_field_item... Ts>
        template<FStrChar V>
        auto FieldMap<Ts...>::get() const {
            constexpr auto idx = indexOfName_v<V, Ts...>;

            static_assert(idx != -1, "Field not found");

            using T = std::tuple_element_t<idx, std::tuple<Ts...>>;

            return std::pair<typename T::type&, decltype(T::dep)>{std::get<idx>(*tuplePtr), T::dep};
        }

        template<is_field_item... Ts>
        template<std::size_t I>
        auto FieldMap<Ts...>::get() const {
            static_assert(I < sizeof...(Ts), "Index out of range");

            using T = std::tuple_element_t<I, std::tuple<Ts...>>;

            return std::pair<typename T::type&, decltype(T::dep)>{std::get<I>(*tuplePtr), T::dep};
        }

        template<is_field_item... Ts>
        template<typename F>
        void FieldMap<Ts...>::on(const std::string& key, F&& f) const {
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                bool found = false;

                (..., [&] {
                    if (found) return;

                    using T = std::tuple_element_t<Is, std::tuple<Ts...>>;

                    if (std::string_view(T::name.data.data(), T::name.size) == key) {
                        f(std::get<Is>(*tuplePtr), T::dep);

                        found = true;
                    }
                }());

                if (!found) throw std::runtime_error("Field not found: " + key);
            }(std::make_index_sequence<sizeof...(Ts)>{});
        }

        template<is_field_item... Ts>
        template<FStrChar V>
        constexpr bool FieldMap<Ts...>::has() const {
            return indexOfName_v<V, Ts...> != -1;
        }

        template<is_field_item... Ts>
        template<std::size_t N>
        constexpr bool FieldMap<Ts...>::has(FStrChar<N> v) const {
            return std::apply([&](const auto&... fs) { return !(... || (fs.name != v)); }, keys);
        }

        template<is_field_item... Ts>
        template<typename F>
        void FieldMap<Ts...>::forEach(F&& f) {
            ([&, this]<is_field_item T>(T&&) { f.template operator()<T::name, typename T::type, T::dep>(*tuplePtr++); }(Ts{}), ...);
        }

    }  // namespace detail

    inline Package<> Package<>::compressDeserializeImpl(const std::byte* data) {
        // 解析数据包长度
        auto [packetLen, packetLenShift] = parseVarInt<int>(data);
        data += packetLenShift;

        // 解析数据长度
        auto [dataLen, dataLenShift] = parseVarInt<int>(data);
        data += dataLenShift;
        packetLen -= dataLenShift;

        std::vector<std::byte> dataVec{data, data + packetLen};
        if (dataLen) {  // 判断是否启用压缩
#ifdef DEBUG
            Debugger decompressDataDbg(&decompressData);
            dataVec = decompressDataDbg(dataVec, dataLen);
#else
            dataVec = decompressData(dataVec, dataLen);
#endif
        }

        // 解析数据包ID
        auto [id, idShift] = parseVarInt<int>(dataVec.data());

        data         = dataVec.data() + idShift;
        auto resSize = dataVec.size() - idShift;

        std::vector resVec(data, data + resSize);

        return {id, std::move(resVec), resSize};
    }

    inline Package<> Package<>::uncompressDeserializeImpl(const std::byte* data) {
        // 解析数据包长度
        auto [len, lenShift] = parseVarInt<int>(data);
        data += lenShift;

        // 解析数据包ID
        auto [id, idShift] = parseVarInt<int>(data);
        data += idShift;
        len -= idShift;

        return {id, std::vector(data, data + len), static_cast<std::size_t>(len)};
    }

    inline Package<>::Package(const int id, std::vector<std::byte>&& data, const std::size_t size)
        : id_(id)
        , data_(std::move(data))
        , size_(size) {}

    inline int Package<>::id() const { return id_; }

    inline std::vector<std::byte> Package<>::data() const { return data_; }

    inline std::size_t Package<>::size() const { return size_; }

    inline auto Package<>::deserialize(const std::byte* data, bool compressed) { return compressed ? compressDeserializeImpl(data) : uncompressDeserializeImpl(data); }

    inline std::string Package<>::toString() const {
        std::stringstream ss;

        ss << "{ id: " << id_ << ", data: ";

        for (std::size_t i{0}; i < size_; ++i)
            if (const auto c = static_cast<unsigned char>(data_[i]); std::isprint(c))
                ss << c;
            else
                ss << (i == 0 ? "" : " ") << "\\0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data_[i]);

        ss << " }";

        return ss.str();
    }

    inline std::string Package<>::toHexString() const { return minecraft::toHexString(data_); }

    // Fixed package

    template<int I, is_field_item... Ts>
    std::vector<std::byte> Package<I, Ts...>::compressSerializeImpl(int threshold) const {
        std::vector<std::byte> data;

        auto idBytes = VarInt(I).serialize();
        data.insert_range(data.end(), idBytes);

        detail::forEach(fields_, [&data](const auto& f) {
            auto fieldBytes = f.serialize();

            data.insert_range(data.end(), fieldBytes);
        });

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

    template<int I, is_field_item... Ts>
    std::vector<std::byte> Package<I, Ts...>::uncompressSerializeImpl() const {
        std::vector<std::byte> result = VarInt(I).serialize();

        detail::forEach(fields_, [&result](const auto& f) {
            auto fieldBytes = f.serialize();

            result.insert_range(result.end(), fieldBytes);
        });

        const auto sizeBytes = VarInt(static_cast<int>(result.size())).serialize();

        result.insert_range(result.begin(), sizeBytes);

        return result;
    }

    template<int I, is_field_item... Ts>
    Package<I, Ts...> Package<I, Ts...>::compressDeserializeImpl(const std::byte* data) {
        // 解析数据包长度
        auto [packetLen, packetLenShift] = parseVarInt<int>(data);
        data += packetLenShift;

        // 解析数据长度
        auto [dataLen, dataLenShift] = parseVarInt<int>(data);
        data += dataLenShift;
        packetLen -= dataLenShift;

        std::vector<std::byte> dataVec{data, data + packetLen};
        if (dataLen) {  // 判断是否启用压缩
#ifdef DEBUG
            Debugger decompressDataDbg(&decompressData);
            dataVec = decompressDataDbg(dataVec, dataLen);
#else
            dataVec = decompressData(dataVec, dataLen);
#endif
        }

        data = dataVec.data();

        // 解析数据包ID
        auto [id, idShift] = parseVarInt<int>(data);
        data += idShift;

        if (id != I) throw std::runtime_error("PackageImpl ID mismatch after decompression.");

        std::tuple<typename Ts::type...> fields{};

        std::size_t offset = 0;

        // 解析字段
        forEachType([&]<FStrChar V, is_field T, detail::is_nullable_fstr auto D>() {
            constexpr auto idx = indexOfName_v<V, Ts...>;

            if constexpr (D == Null)
                std::get<idx>(fields) = T::deserialize(data + offset);

            else if constexpr (*D == "__rest__")
                std::get<idx>(fields) = T::deserialize(data + offset, dataLen - offset);

            else {
                constexpr auto depIdx = indexOfName_v<*D, Ts...>;

                static_assert(depIdx != -1, "Dependency not found.");

                auto dep = std::get<static_cast<std::size_t>(depIdx)>(fields);

                std::get<idx>(fields) = T::deserialize(data + offset, dep);
            }

            offset += std::get<idx>(fields).size();
        });

        // if (offset != dataVec.size() - idShift)
        //     std::cerr << "Warning: [Package::deserialize] Package data mismatch after decompression. Expected " << dataVec.size() - idShift << " bytes, Actual: " << offset << " bytes." << std::endl;

        return Package(fields);
    }

    template<int I, is_field_item... Ts>
    Package<I, Ts...> Package<I, Ts...>::uncompressDeserializeImpl(const std::byte* data) {
        // 解析数据包长度
        auto [len, lenShift] = parseVarInt<int>(data);
        data += lenShift;

        // 解析数据包ID
        auto [id, idShift] = parseVarInt<int>(data);
        data += idShift;
        len -= idShift;

        std::tuple<typename Ts::type...> fields{};

        std::size_t offset = 0;

        // 解析字段
        forEachType([&]<FStrChar V, is_field T, detail::is_nullable_fstr auto D>() {
            constexpr auto idx = indexOfName_v<V, Ts...>;

            if constexpr (D == Null)
                std::get<idx>(fields) = T::deserialize(data + offset);

            else if constexpr (*D == "__rest__")
                std::get<idx>(fields) = T::deserialize(data + offset, len - offset);

            else {
                constexpr auto depIdx = indexOfName_v<*D, Ts...>;

                static_assert(depIdx != -1, "Dependency not found.");

                auto dep = std::get<static_cast<std::size_t>(depIdx)>(fields);

                std::get<idx>(fields) = T::deserialize(data + offset, dep);
            }

            offset += std::get<idx>(fields).size();
        });

        // if (offset != len) std::cerr << "Warning: [Package::deserialize] Package data mismatch. Expected " << len << " bytes, Actual: " << offset << " bytes." << std::endl;

        return Package(fields);
    }

    template<int I, is_field_item... Ts>
    Package<I, Ts...>::Package(std::tuple<typename Ts::type...> fields)
        : fields_(std::move(fields))
        , fieldMap_(fields_) {}

    template<int I, is_field_item... Ts>
    Package<I, Ts...>::Package(typename Ts::type&&... args)
        : fields_{std::forward<typename Ts::type>(args)...}
        , fieldMap_(fields_) {}

    template<int I, is_field_item... Ts>
    template<typename F>
    void Package<I, Ts...>::onField(std::string key, F&& f) const {
        [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            (void)(... || (std::get<Is>(names).equals(key.data(), key.size()) ? (f(std::get<Is>(fields_)), true) : false));
        }(std::make_index_sequence<size>{});
    }

    template<int I, is_field_item... Ts>
    template<typename F>
    void Package<I, Ts...>::forEachField(F&& f) {
        ([&, this]<is_field_item T>(T&&) { f.template operator()<T::name, typename T::type, T::dep>(std::get<indexOfName_v<T::name, Ts...>>(fields_)); }(Ts{}), ...);
    }

    template<int I, is_field_item... Ts>
    template<typename F>
    void Package<I, Ts...>::forEachType(F&& f) {
        ([&]<is_field_item T>(T&&) { f.template operator()<T::name, typename T::type, T::dep>(); }(Ts{}), ...);
    }

    template<int I, is_field_item... Ts>
    template<FStrChar V>
    auto Package<I, Ts...>::get() const {
        constexpr auto idx = indexOfName_v<V, Ts...>;

        static_assert(idx != -1, "Field not found.");

        return std::get<idx>(fields_);
    }

    template<int I, is_field_item... Ts>
    auto Package<I, Ts...>::serialize(const bool compressed, const int threshold) const {
        if (data_.empty()) data_ = compressed ? compressSerializeImpl(threshold) : uncompressSerializeImpl();

        return data_;
    }

    template<int I, is_field_item... Ts>
    auto Package<I, Ts...>::deserialize(const std::byte* data, bool compressed) {
        return compressed ? compressDeserializeImpl(data) : uncompressDeserializeImpl(data);
    }

    template<int I, is_field_item... Ts>
    std::string Package<I, Ts...>::toString() const {
        std::stringstream ss;

        ss << "{ id: " << id << ", ";

        [&ss, this]<std::size_t... Is>(std::index_sequence<Is...>) {
            (..., (ss << std::string_view(std::get<Is>(names).data.data(), std::get<Is>(names).size) << ": " << std::get<Is>(fields_).toString() << (Is < size - 1 ? ", " : "")));
        }(std::make_index_sequence<size>{});

        ss << " }";

        return ss.str();
    }

    template<int I, is_field_item... Ts>
    std::string Package<I, Ts...>::toHexString() const {
        return minecraft::toHexString(data_);
    }

    // Unkown package


}  // namespace minecraft::protocol

#endif  // PACKAGE_HPP
