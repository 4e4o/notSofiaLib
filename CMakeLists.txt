cmake_minimum_required(VERSION 3.5)

project(notSofiaLib LANGUAGES CXX C)

set(HI_SDK_PATH "/mnt/disk2/workspace/devices/mars/dvr/hisilicon_sdk_and_tools/Hi3520D_SDK_V1.0.5.1")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty")

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

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    )

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    ${HI_SDK_INCLUDE}
    ${EXT_DRV_DIRS}
    ${THIRD_PARTY_DIR}
    )

target_link_libraries(${PROJECT_NAME} PUBLIC ${LINK_LIBS})

function(set_elf_rpath_and_ld_path)
    set(SYSTEM_NEW_TOOLCHAIN_LIBS_PATH "/usr/lib/notSofia")

    # тулчёйн самосборный, либы все новые и их надо в отдельное место в системе положить
    # и бинарнику сказать где искать это всё
    set_target_properties(${ARGV0} PROPERTIES LINK_FLAGS
        "-Wl,-rpath,${SYSTEM_NEW_TOOLCHAIN_LIBS_PATH} -Wl,--dynamic-linker=${SYSTEM_NEW_TOOLCHAIN_LIBS_PATH}/ld-uClibc.so.1")
endfunction()

