#!/bin/bash

set -x
set -e
cd `dirname "$0"`

generate_shaders()
{
    platform=$1
    xcrun -sdk macosx metal -c ./shader.metal -o shader.air
    xcrun -sdk macosx metallib shader.air -o shader.metallib
    xxd -i shader.metallib | perl -w -p -e 's/\Aunsigned /const unsigned /;' >./shaders_metal_$platform.h
    rm -f shader.air shader.metalar shader.metallib
}

generate_shaders osx
#generate_shaders ios

