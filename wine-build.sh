#!/bin/bash

docker run --rm -v "$(readlink -f .)":/bbpp -w /bbpp msvc:latest bash -c \
"/opt/msvc/bin/x86/cl /nologo /LD /EHsc /I. Blue\ Burst\ Patch\ Project/*.cpp $* /link /OUT:bbpp.dll /subsystem:console User32.lib;
chown $(id -u $USER):$(id -g $USER) bbpp.dll"
