#!/bin/bash
for file in shaders/glsl/*
do 
filename=$(basename $file)
filename_noext="${filename%%.*}"
glslc shaders/glsl/$filename -o shaders/spv/$filename_noext.spv
done
cd build && ninja
cd ..