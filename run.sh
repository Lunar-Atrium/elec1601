#!/usr/bin/bash

SRCDIR=src
LIBDIR=lib

gcc $SRCDIR/*.c -I $LIBDIR -g -o main `sdl2-config --cflags --libs` -lm
./main