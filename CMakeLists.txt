cmake_minimum_required(VERSION 3.5)

project(notSofiaLib LANGUAGES CXX C)

set(HI_SDK_PATH "/mnt/disk2/workspace/devices/mars/dvr/hisilicon_sdk_and_tools/Hi3520D_SDK_V1.0.5.1")
set(NVP6134_PATH "${HI_SDK_PATH}/../NVP6134C/02_SDK/")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC "")

set(HI_SDK_INCLUDE "${HI_SDK_PATH}/mpp/include")
set(HI_SDK_LIBS_DIR "${HI_SDK_PATH}/mpp/lib")
set(EXT_DRV_PATH "${HI_SDK_PATH}/mpp/extdrv")

set(EXT_DRV_DIRS
    "${EXT_DRV_PATH}/gpio_i2c"
    )

set(LINK_LIBS
    "-L ${HI_SDK_LIBS_DIR}"
    "libmpi.a"
    "libaec.a"
    "libanr.a"
    "libtde.a"
    "libvqev2.a"
    "libresampler.a"
    "libVoiceEngine.a"
    "libjpeg6b.a"
    )

file(GLOB_RECURSE PRJ_SRCS src/*.c src/*.cpp src/*.h src/*.hpp notSofia.astylerc)
target_sources(${PROJECT_NAME} PRIVATE ${PRJ_SRCS})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${HI_SDK_INCLUDE}
    ${EXT_DRV_DIRS}
    ${NVP6134_PATH}
    )

target_link_libraries(${PROJECT_NAME} PUBLIC ${LINK_LIBS})
