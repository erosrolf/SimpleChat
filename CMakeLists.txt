cmake_minimum_required(VERSION 3.30)
project(SimpleChat)

set(CMAKE_CXX_STANDARD 20)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXT_LIBS_DIR ${CMAKE_SOURCE_DIR}/ext-libs)

add_compile_definitions(ASIO_STANDALONE)

include_directories(
        ${EXT_LIBS_DIR}/asio/asio/include
        ${EXT_LIBS_DIR}/websocketpp
        ${EXT_LIBS_DIR}/absl
)

add_executable(SimpleChat
        ${SRC_DIR}/chat_server_main.cc
        ${SRC_DIR}/ws/ChatServer.cc
)

add_subdirectory(${EXT_LIBS_DIR}/absl)
target_link_libraries(SimpleChat PRIVATE absl::synchronization pthread)