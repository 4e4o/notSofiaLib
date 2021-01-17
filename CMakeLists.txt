cmake_minimum_required(VERSION 3.5)

project(notSofia LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} "")

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    )

file(GLOB PRJ_SRCS src/*.cpp src/*.h src/*.hpp)
target_sources(${PROJECT_NAME} PRIVATE ${PRJ_SRCS})
target_link_libraries(${PROJECT_NAME} pthread)
