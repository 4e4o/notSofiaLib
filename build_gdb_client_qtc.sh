#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# пересобираем gdb клиент с поддержкой питона чтоб qt creator мог юзать его
# для быстрой отладки

git clone --depth=1 git://sourceware.org/git/binutils-gdb.git
rm -rf gdb_install
mkdir gdb_install
cd binutils-gdb

./configure --target=arm-linux-gnueabi --with-python --prefix="${DIR}/gdb_install"
make -j5
make install
