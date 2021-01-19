cmake_minimum_required(VERSION 3.5)

project(notSofia LANGUAGES CXX C)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} "")

set(HI_SDK_PATH "/home/computer/workspace/info/manuals/hisilicon_dvr/hisilicon_sdk/Hi3520D_SDK_V1.0.5.1")

set(HI_SDK_INCLUDE "${HI_SDK_PATH}/mpp/include")
set(HI_SDK_LIBS_DIR "${HI_SDK_PATH}/mpp/lib")
set(EXT_DRV_PATH "${HI_SDK_PATH}/mpp/extdrv")

set(EXT_DRV_DIRS
    "${EXT_DRV_PATH}/cx26828"
    "${EXT_DRV_PATH}/gpio_i2c"
    "${EXT_DRV_PATH}/nvp6114"
    "${EXT_DRV_PATH}/ssp-GV7601"
    "${EXT_DRV_PATH}/tlv320aic31"
    "${EXT_DRV_PATH}/tw2865"
    "${EXT_DRV_PATH}/tw2868"
    "${EXT_DRV_PATH}/tw2960"
    "${EXT_DRV_PATH}/tlv320aic31"
    )

set(LINK_LIBS
    "-L ${HI_SDK_LIBS_DIR}"
    "libc.a"
    "libmpi.a"
    "libhdmi.a"
    "libjpeg6b.a"
    "libresampler.a"
    "libVoiceEngine.a"
    "libaec.a"
    "libanr.a"
    "libtde.a"
    "libvqev2.a"
    )

#DEMO
target_compile_definitions(${PROJECT_NAME} PUBLIC hi3520D HICHIP=0x3520D100 HI_DEBUG HI_XXXX)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/common
    ${HI_SDK_INCLUDE}
    ${EXT_DRV_DIRS}
    "/home/computer/workspace/info/manuals/hisilicon_dvr/hisilicon_sdk/NVP6134C/02_SDK/"
    )

file(GLOB_RECURSE PRJ_SRCS src/*.c src/*.cpp src/*.h src/*.hpp)
target_sources(${PROJECT_NAME} PRIVATE ${PRJ_SRCS})
target_link_libraries(${PROJECT_NAME} pthread ${LINK_LIBS})
