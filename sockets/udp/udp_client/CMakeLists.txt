cmake_minimum_required(VERSION 3.5.0)

project(udp_client)

file(GLOB SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})

