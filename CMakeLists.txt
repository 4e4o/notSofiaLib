cmake_minimum_required(VERSION 3.5)

project(notSofia LANGUAGES CXX C)

set(CMAKE_VERBOSE_MAKEFILE ON)

# sudo dnf install gcc-c++-arm-linux-gnueabi
# used compiler:
# blabla/Hi3520D_SDK_V1.0.5.1/osdrv/toolchain/arm-hisiv100nptl-linux/install/opt/hisi-linux-nptl/arm-hisiv100-linux/bin/arm-hisiv100-linux-uclibcgnueabi-gcc

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} "")

set(HI_SDK_PATH "/mnt/disk2/workspace/devices/mars/dvr/hisilicon_sdk_and_tools/Hi3520D_SDK_V1.0.5.1")
set(NVP6134_PATH "${HI_SDK_PATH}/../NVP6134C/02_SDK/")

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

target_compile_definitions(${PROJECT_NAME} PUBLIC HICHIP=0x3520D100)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/common
    ${HI_SDK_INCLUDE}
    ${EXT_DRV_DIRS}
    ${NVP6134_PATH}
    )

file(GLOB_RECURSE PRJ_SRCS src/*.c src/*.cpp src/*.h src/*.hpp)
target_sources(${PROJECT_NAME} PRIVATE ${PRJ_SRCS})
target_link_libraries(${PROJECT_NAME} pthread ${LINK_LIBS})
