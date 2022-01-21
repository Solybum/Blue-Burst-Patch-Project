#!/bin/bash

src_dir="Blue Burst Patch Project"
libs="User32.lib"

if [[ "$*" == *"PATCH_NEWENEMY"* ]]; then
    libs="$libs lib/assimp-vc142-mt.lib lib/zlibstatic.lib Advapi32.lib"
fi

cmd="/opt/msvc/bin/x86/cl /nologo /LD /EHsc /MD /DCINTERFACE \
/I'$src_dir' /I'$src_dir'/newgfx /Iinclude \
'$src_dir'/*.cpp '$src_dir'/**/*.cpp $* \
/link /OUT:bbpp.dll /subsystem:console $libs && \
chown $(id -u $USER):$(id -g $USER) bbpp.dll"

echo "$cmd"

docker run --rm -v "$(readlink -f .)":/bbpp -w /bbpp msvc:latest bash -c "$cmd"
