#!/bin/zsh
for filename in ./*.hlsl; do
    echo "loop"
        ~/bin/shadercross "$filename" -o "../Compiled/SPIRV/${filename/.hlsl/.spv}"
        ~/bin/shadercross "$filename" -o "../Compiled/MSL/${filename/.hlsl/.msl}"
        ~/bin/shadercross "$filename" -o "../Compiled/DXIL/${filename/.hlsl/.dxil}"
done
