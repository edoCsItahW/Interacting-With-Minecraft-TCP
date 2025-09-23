# Minecraft-TCP Client Library

[![Language: C++](https://img.shields.io/badge/Language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/C++-17/20-ff69b4.svg)](https://en.cppreference.com/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey.svg)]()

一个高性能、类型安全的 C++ 库，用于与 Minecraft Java Edition 服务器建立和管理完整的 TCP 连接。本项目实现了官方的 Minecraft 网络协议，使开发者能够构建功能强大的自定义客户端、机器人或监控工具。

## ✨ 核心特性

*   **类型安全的协议实现**: 基于模板的编译时数据包验证系统 (`Package<ID, FIELDS>`)，确保数据包结构和类型的正确性，最大程度减少运行时错误。
*   **完整的协议状态支持**: 全面支持从`握手` -> `状态` -> `登录` -> `游戏`的完整协议生命周期，处理所有客户端与服务器间的数据包交互。
*   **跨平台网络层**: 抽象化的 Socket 操作，无缝支持 Windows (Winsock2) 和 Unix-like (POSIX) 系统，包含自动的平台初始化和清理。
*   **高效的多线程架构**: 采用生产者-消费者模式，使用独立的发送和接收线程处理网络 I/O，保证通信流畅且响应迅速。
*   **生产就绪的设计**: 遵循 RAII 原则，使用现代 C++（17/20）特性，包含智能指针管理、结构化日志系统以及可配置的调试模式。
*   **可扩展的类型系统**: 提供一套完备的类型（如 `Integer`, `String`, `UUID`, `VarNum`）和实用工具，便于序列化、反序列化和验证协议数据。

## 🏗️ 系统架构概述

该项目采用分层架构，将不同的关注点分离到明确的模块中：

```
+-----------------------+
|   Application Layer   |  <-- 你的自定义客户端逻辑或机器人
+-----------------------+
            |
+-----------------------+
|   Client Communication|  <-- Client, ClientBase 类 (连接管理, 线程, 队列)
+-----------------------+
            |
+-----------------------+
|    Protocol System    |  <-- 数据包解析、序列化、状态机 (parsePacket, Package<>)
+-----------------------+
            |
+-----------------------+
|      Type System      |  <-- 类型安全的基元与复杂类型 (Boolean, Integer, String, etc.)
+-----------------------+
            |
+-----------------------+
| Network Infrastructure|  <-- 跨平台 Socket 抽象 (SOCKET, 线程原语)
+-----------------------+
```

### 核心组件

| 组件 | 主要类/函数 | 职责 |
| :--- | :--- | :--- |
| **客户端通信** | `Client`, `ClientBase<T>` | TCP 连接管理、线程、消息队列 |
| **协议处理** | `parsePacket()`, `Package<ID, FIELDS>` | 协议状态管理、数据包解析与构建 |
| **类型系统** | `Boolean<V>`, `Integer<T,V>`, `String<V>`, `UUID<V>`, `VarNum<T,V>` | 类型安全的数据序列化与验证 |
| **网络基础设施** | `SOCKET`, 线程原语 | 底层网络通信和平台抽象 |

## 🚀 快速开始

###  prerequisites

*   **C++ 编译器**: 支持 C++17 或更高版本的编译器 (如 GCC >= 8, Clang >= 7, MSVC >= 2019)
*   **构建系统**: [CMake](https://cmake.org/) (版本 3.12 或更高)
*   **可选: 测试框架**: 项目可能依赖如 Google Test 等框架用于单元测试。

### 构建与安装

1.  **克隆仓库**
    ```bash
    git clone <your-repository-url>
    cd minecraft-tcp
    ```

2.  **配置并构建**
    ```bash
    # 创建一个构建目录并进入
    mkdir build && cd build

    # 配置 CMake
    # 使用 -DCMAKE_BUILD_TYPE=Release 用于发布版本
    cmake .. -DCMAKE_BUILD_TYPE=Debug

    # 开始构建
    cmake --build . --parallel
    ```
    构建后的库文件和可执行文件将位于 `build` 目录下。

### 基本用法

以下是一个简单的示例，展示如何使用该库连接到服务器并完成握手。

```cpp
#include <client/client.h>
#include <protocol/package/definition.h>
#include <iostream>

int main() {
    try {
        // 1. 创建一个客户端实例
        minecraft::Client client;

        // 2. 连接到服务器
        client.connect("mc.hypixel.net", 25565);

        // 3. 发送握手包 (示例)
        // 假设 ProtocolState 和 Package 类型已定义
        auto handshake_pkt = minecraft::protocol::create_handshake_packet(
            /* protocol_version */ 762,
            /* server_address */ "mc.hypixel.net",
            /* server_port */ 25565,
            /* next_state */ minecraft::protocol::ProtocolState::Login
        );
        client.sendPacket(handshake_packet);

        // ... 处理后续登录逻辑

        std::cout << "Connected successfully!" << std::endl;

        // 4. 进入主循环或处理事件
        while (client.isConnected()) {
            // 处理接收到的数据包...
            // client.receive(); // 通常由后台线程自动处理
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```
*请注意：以上代码为概念性示例，具体函数名和创建方法请参考实际代码库的API。*

## 📚 文档

更深入的文档可以在代码库的 `docs/` 目录下找到，或者通过内联的 Doxygen 注释生成。

*   **[开发环境](./docs/development_environment.md)**: 设置开发环境的详细指南。
*   **[协议系统](./docs/protocol_system.md)**: 详细解释数据包定义、解析和状态机。
*   **[类型系统](./docs/type_system.md)**: 所有可用类型及其用法的完整参考。
*   **[客户端通信](./docs/client_communication.md)**: 多线程架构和网络管理的细节。

## 🔧 扩展项目

### 添加新的数据包

1.  **在 `protocol/package/definition.h` (或相应文件) 中定义数据包**：
    ```cpp
    // 1. 定义数据包ID（如果需要）
    // constexpr int MY_NEW_PACKET_ID = 0x42;

    // 2. 定义数据包结构
    using MyNewServerBoundPacket = Package<
        MY_NEW_PACKET_ID,
        Field<"SomeField", String<>>,
        Field<"Value", VarInt>
    >;

    // 3. 在正确的协议状态中注册它
    // template <>
    // struct PacketMapping<ProtocolState::Play, ServerBound> {
    //     ...
    //     using type = std::variant<..., MyNewServerBoundPacket>;
    // };
    ```

2.  **实现序列化/反序列化逻辑**（如果使用自定义类型）。
3.  **更新状态机映射**，确保新数据包在正确的协议状态下被解析和处理。

### 添加新的数据类型

1.  创建一个新的类型模板，并为其实现 `serialize` 和 `deserialize` 方法。
2.  确保它符合库类型系统所要求的概念（如 `Serializable`）。
3.  在 `type system` 文档中记录它的用法。

## 🤝 如何贡献

我们欢迎任何形式的贡献！请随时 Fork 本项目并提交 Pull Request。

1.  Fork 本仓库
2.  创建您的特性分支 (`git checkout -b feature/AmazingFeature`)
3.  提交您的更改 (`git commit -m 'Add some AmazingFeature'`)
4.  推送到分支 (`git push origin feature/AmazingFeature`)
5.  开启一个 Pull Request

在贡献之前，请阅读 `CONTRIBUTING.md`（如果存在）以了解我们的编码规范和测试要求。

## 📄 许可证

本项目基于 **MIT 许可证** - 查看 [LICENSE](LICENSE) 文件了解详情。

## 🙏 致谢

*   感谢 [Mojang Studios](https://www.minecraft.net/) 提供了 Minecraft 及其公开的网络协议规范。
*   感谢所有为此项目做出贡献的开发者。

## 🛠️ 技术支持

如果您遇到任何问题或有疑问，请采取以下步骤：
1.  **首先**，查看 [文档](./docs) 和现有的 [Git Issues](issues)。
2.  如果找不到答案，请**开启一个新 Issue**，清晰描述您的问题、错误日志以及复现步骤。

---

**Happy Coding!** 🎮