cmake_minimum_required(VERSION 3.15)
project(ollama_json_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json REQUIRED)

add_executable(ollama_json_example main.cpp)
target_link_libraries(ollama_json_example PRIVATE nlohmann_json::nlohmann_json)

# Copy config.json to build directory
configure_file(
    ${CMAKE_SOURCE_DIR}/config.json
    ${CMAKE_BINARY_DIR}/config.json
    COPYONLY
)