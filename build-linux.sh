#!/bin/bash

set echo off

export BUILD_DIR=build
export BUILD_TYPE=Debug
export SHARED_LIB=OFF

if [ -d $BUILD_DIR ]; then
        rm -rf $BUILD_DIR
fi

mkdir $BUILD_DIR
cd $BUILD_DIR

cmake .. -G "Unix Makefiles"               \
	-DOPERATING_SYSTEM:STRING=LINUX          \
	-DARCH_BITS:STRING=32                    \
	-DBUILD_SHARED_LIBS:BOOL=$SHARED_LIB     \
	-DCMAKE_BUILD_TYPE:STRING=$BUILD_TYPE    \
	-DBUILD_TYPE:STRING=$BUILD_TYPE          \
	-DCMAKE_ENABLE_EXPORTS:BOOL=ON

cmake --build . --config $BUILD_TYPE

cd ../

if [ -d $BUILD_DIR ]; then
	rm -rf $BUILD_DIR
fi
