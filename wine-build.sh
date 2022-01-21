#!/bin/bash

src_dir="Blue Burst Patch Project"

docker run --rm -v "$(readlink -f .)":/bbpp -w /bbpp msvc:latest bash -c \
"/opt/msvc/bin/x86/cl /nologo /LD /EHsc /MT /DCINTERFACE \
    /I'$src_dir' /I'$src_dir'/newgfx /Iinclude \
    '$src_dir'/*.cpp '$src_dir'/**/*.cpp $* \
    /link /OUT:bbpp.dll /subsystem:console User32.lib lib/assimp-vc142-mt.lib &&
chown $(id -u $USER):$(id -g $USER) bbpp.dll"
