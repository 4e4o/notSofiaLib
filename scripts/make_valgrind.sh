#!/bin/sh

#PPP="/home/computer/x-tools/arm-unknown-linux-uclibcgnueabi/bin"
#TTT="arm-unknown-linux-uclibcgnueabi"

PPP="/mnt/disk2/workspace/devices/mars/dvr/hisilicon_sdk_and_tools/Hi3520D_SDK_V1.0.5.1/osdrv/toolchain/arm-hisiv100nptl-linux/install/opt/hisi-linux-nptl/arm-hisiv100-linux/bin"
TTT="arm-hisiv100-linux-uclibcgnueabi"



PTT=$PPP/$TTT

export AR=$PTT-ar
export LD=$PTT-ld
export CC=$PTT-gcc
export CXX=$PTT-g++
export RANLIB=$PTT-ranlib

export PATH=$PATH:$PPP

./configure --host=$TTT --target=$TTT --prefix=/mnt/disk2/workspace/coding/valgrind_install


# to run valgrind:
#export VALGRIND_LIB=/root/dev/valgrind_install/libexec/valgrind/
#cd /root/dev/valgrind_install/bin/
#./valgrind
