#! /bin/bash

cp_to_libs() {
    for file in $parent
        do
            cp -R $file $target
        done
}
## build miniglog
cd native/third_party/miniglog/example
ndk-build NDK_DEBUG=0
parent=../jniLibs/*
target=../../../../libs
cp_to_libs $parent $target

## build libpng
cd ../../../third_party/libpng/android
ndk-build NDK_DEBUG=0
parent=jniLibs/*
target=../../../../libs
cp_to_libs $parent $target

## build dlib
cd ../../../dlib/android
ndk-build NDK_DEBUG=0
parent=jniLibs/*
target=../../../libs
cp_to_libs $parent $target

## build seetaface
cd ../../seetaface
ndk-build NDK_DEBUG=0
parent=libs/*
target=../../libs
cp_to_libs $parent $target