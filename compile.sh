#!/bin/bash
for file in shaders/glsl/*
do 
filename=$(basename $file)
glslc shaders/glsl/$filename -o shaders/spv/$filename.spv
done
cd build && ninja
cd ..