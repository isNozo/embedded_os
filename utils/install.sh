#!/bin/sh

BASE=$(pwd)
BINUTILS=binutils-2.19.1
GCC=gcc-3.4.6

cd $BASE
tar xvzf $BINUTILS.tar.gz
cd $BINUTILS
./configure --target=h8300-elf --disable-nls --disable-werror
make
make install
cd $BASE
rm -rf $BINUTILS

cd $BASE
tar xvzf $GCC.tar.gz
cd $GCC
patch gcc/config/h8300/h8300.c < ../gcc-3.4.6-x64-h8300.patch
./configure --target=h8300-elf --disable-nls --disable-threads --disable-shared --enable-languages=c --disable-werror
make
make install
cd $BASE
rm -rf $GCC
