cmake_minimum_required(VERSION 3.27)
project(creature_ai_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



include(FetchContent)
set(FETCHCONTENT_QUIET OFF)


# Find system libcurl
find_package(CURL REQUIRED)

# fmt
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 9.1.0
)
FetchContent_MakeAvailable(fmt)
set(FMT_HEADER_ONLY ON)
set(FMT_LOCALE ON)


# spdlog
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.11.0
)
FetchContent_MakeAvailable(spdlog)

# JSON
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)


add_executable(creature_ai_engine
        src/main.cpp
        src/AssistantConfig.h)

target_link_libraries(creature_ai_engine
        PUBLIC
        fmt::fmt
        spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
        nlohmann_json::nlohmann_json
        CURL::libcurl
)