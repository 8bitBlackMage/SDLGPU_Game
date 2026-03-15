#!/bin/zsh
echo "deleting old shaders"
rm -rf ../Compiled/SPIRV/*
rm -rf ../Compiled/MSL/*
rm -rf ../Compiled/DXIL/*

for filename in ./*.hlsl; do
    echo "$filename"
        ~/bin/shadercross "$filename" -o "../Compiled/SPIRV/${filename/.hlsl/.spv}"
        ~/bin/shadercross "$filename" -o "../Compiled/MSL/${filename/.hlsl/.msl}"
        ~/bin/shadercross "$filename" -o "../Compiled/DXIL/${filename/.hlsl/.dxil}"
done
