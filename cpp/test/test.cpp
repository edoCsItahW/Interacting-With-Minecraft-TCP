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

    for (auto byte : containter) ss << "\\0x" << std::setw(2) << static_cast<int>(byte) << " ";

    std::cout << ss.str() << std::endl;
}

void varNum_test() {
    using namespace minecraft;
    // 运行期VarInt测试

    auto rVarInt = protocol::VarInt(25565);

    auto rVarIntBytes = rVarInt.encode();

    std::cout << "VarInt encoded bytes: " << std::endl;
    print_bytes(rVarIntBytes);

    auto rVarIntDeencoded = protocol::VarInt::decode(rVarIntBytes.data());

    std::cout << "VarInt decoded value: " << rVarIntDeencoded.value() << std::endl << std::endl;
}

void integer_test() {
    using namespace minecraft::protocol;

    // 运行期Int测试

    auto rInt = Int(25565);

    auto rIntBytes = rInt.encode();

    std::cout << "Int encoded bytes: " << std::endl;
    print_bytes(rIntBytes);

    auto rIntDeencoded = Int::decode(rIntBytes.data());
    std::cout << "Int decoded value: " << rIntDeencoded.value() << std::endl << std::endl;

    // 运行期UShort测试

    auto rUShort = UShort(25565);

    auto rUShortBytes = rUShort.encode();

    std::cout << "UShort encoded bytes: " << std::endl;
    print_bytes(rUShortBytes);

    auto rUShortDeencoded = UShort::decode(rUShortBytes.data());

    std::cout << "UShort decoded value: " << rUShortDeencoded.value() << std::endl << std::endl;
}

void str_test() {
    using namespace minecraft::protocol;
    // 运行期Str测试

    auto rStr = String("Hello, world!");

    auto rStrBytes = rStr.encode();

    std::cout << "String encoded bytes: " << std::endl;
    print_bytes(rStrBytes);

    auto rStrDeencoded = String::decode(rStrBytes.data());

    std::cout << "String decoded value: " << rStrDeencoded.value() << std::endl << std::endl;
}

void mcuuid_test() {
    using namespace minecraft;
    // 运行期MCUUID测试
    auto rMCUUID = protocol::UUID(protocol::genUUID("petter"));

    auto rMCUUIDBytes = rMCUUID.encode();

    std::cout << "MCUUID encoded bytes: " << std::endl;
    print_bytes(rMCUUIDBytes);

    auto rMCUUIDDeencoded = protocol::UUID::decode(rMCUUIDBytes.data());

    std::cout << "MCUUID decoded value: " << rMCUUIDDeencoded.toString() << std::endl << std::endl;
}

void package_test() {
    using namespace minecraft;
    protocol::client_bound::handshake_step::HandShakePacketType handShake{protocol::VarInt(765), protocol::String("localhost"), protocol::UShort(25565), protocol::VarInt(2)};

    auto handShakeBytes = handShake.serialize(false, -1);

    std::cout << "HandShake encoded bytes: " << std::endl;
    print_bytes(handShakeBytes);

    auto handShakeDeencoded = protocol::client_bound::handshake_step::HandShakePacketType::deserialize(handShakeBytes.data());

    std::cout << "HandShake decoded value: " << handShakeDeencoded.toString() << std::endl;
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
