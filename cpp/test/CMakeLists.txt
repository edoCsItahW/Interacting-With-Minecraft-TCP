cmake_minimum_required(VERSION 3.28.1)

project(MC_PROTOCOL_TEST VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_PREFIX_PATH )

# find_package()

#link_directories()

include_directories(
        ${CMAKE_SOURCE_DIR}/../zlib/include
)

if (MSVC)
    add_compile_options(/wd4819)
endif ()

file(GLOB SOURCES "../minecraft/src/*.cpp")

add_executable(minecraft_protocol_test
        ${SOURCES}
        test.cpp
)

target_link_libraries(minecraft_protocol_test
        ${CMAKE_SOURCE_DIR}/../zlib/lib/zlibstatic.lib
)

add_compile_definitions(DEBUG)
