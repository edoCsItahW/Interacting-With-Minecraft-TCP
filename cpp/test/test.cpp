// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file test.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/16 11:23
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */

#include "../minecraft/src/client/client.h"
#include "../minecraft/src/protocol/package/definition.h"
#include "../minecraft/src/protocol/package/package.h"
#include "../minecraft/src/protocol/type/integer.h"
#include "../minecraft/src/protocol/type/mcuuid.h"
#include "../minecraft/src/protocol/type/str.h"
#include "../minecraft/src/protocol/type/varNum.h"
#include "../minecraft/src/utils/utils.h"
#include <iostream>
#include <sstream>


template<typename T>
auto print_bytes(const T& containter) {
    std::stringstream ss;

    ss << std::hex << std::setfill('0');

    for (auto byte : containter) ss << "\\x" << std::setw(2) << static_cast<int>(byte) << " ";

    std::cout << ss.str() << std::endl;
}

void varNum_test() {
    using namespace minecraft;
    // 运行期VarInt测试

    auto rVarInt = protocol::VarInt(25565);

    auto rVarIntBytes = rVarInt.serialize();

    std::cout << "VarInt serialized bytes: " << std::endl;
    print_bytes(rVarIntBytes);

    auto rVarIntDeserialized = protocol::VarInt::deserialize(rVarIntBytes.data());

    std::cout << "VarInt deserialized value: " << rVarIntDeserialized.value() << std::endl << std::endl;

}

void integer_test() {
    using namespace minecraft::protocol;

    // 运行期Int测试

    auto rInt = Int(25565);

    auto rIntBytes = rInt.serialize();

    std::cout << "Int serialized bytes: " << std::endl;
    print_bytes(rIntBytes);

    auto rIntDeserialized = Int::deserialize(rIntBytes.data());
    std::cout << "Int deserialized value: " << rIntDeserialized.value() << std::endl << std::endl;

    // 运行期UShort测试

    auto rUShort = UShort(25565);

    auto rUShortBytes = rUShort.serialize();

    std::cout << "UShort serialized bytes: " << std::endl;
    print_bytes(rUShortBytes);

    auto rUShortDeserialized = UShort::deserialize(rUShortBytes.data());

    std::cout << "UShort deserialized value: " << rUShortDeserialized.value() << std::endl << std::endl;

}

void str_test() {
    using namespace minecraft::protocol;
    // 运行期Str测试

    auto rStr = String("Hello, world!");

    auto rStrBytes = rStr.serialize();

    std::cout << "String serialized bytes: " << std::endl;
    print_bytes(rStrBytes);

    auto rStrDeserialized = String::deserialize(rStrBytes.data());

    std::cout << "String deserialized value: " << rStrDeserialized.value() << std::endl << std::endl;

}

void mcuuid_test() {
    using namespace minecraft;
    // 运行期MCUUID测试
    auto rMCUUID = protocol::UUID(protocol::genUUID("petter"));

    auto rMCUUIDBytes = rMCUUID.serialize();

    std::cout << "MCUUID serialized bytes: " << std::endl;
    print_bytes(rMCUUIDBytes);

    auto rMCUUIDDeserialized = protocol::UUID::deserialize(rMCUUIDBytes.data());

    std::cout << "MCUUID deserialized value: " << rMCUUIDDeserialized.toString() << std::endl << std::endl;

}

void package_test() {
    using namespace minecraft;
    protocol::client_bound::handshake_step::HandShakePacketType handShake{protocol::VarInt(765), protocol::String("localhost"), protocol::UShort(25565), protocol::VarInt(2)};

    auto handShakeBytes = handShake.serialize(false, -1);

    std::cout << "HandShake serialized bytes: " << std::endl;
    print_bytes(handShakeBytes);

    auto handShakeDeserialized = protocol::client_bound::handshake_step::HandShakePacketType::deserialize(handShakeBytes.data());

    std::cout << "HandShake deserialized value: " << handShakeDeserialized.toString() << std::endl;
}

void client_test() {
    using namespace minecraft::client;

    Client client{"localhost", 25565, true};

    client.start();
}


int main() {
    client_test();

    // varNum_test();

    // integer_test();

    // str_test();

    // mcuuid_test();

    // package_test();

    return 0;
}
