#!/bin/bash
build_dir=target
debug_build=debug
release_build=release
mkdir -p $build_dir
cd $build_dir || exit 1
if [ "$1" = "-g" ]
then
    cmake .. -DCMAKE_BUILD_TYPE=Debug -B $debug_build || exit 1
    cd $debug_build || exit 1
    make -j4 || exit 3
else
    cmake .. -DCMAKE_BUILD_TYPE=Release -B $release_build || exit 1
    cd $release_build || exit 1
    make -j4 || exit 2
fi

