#!/bin/bash

clang $(pkg-config --libs --cflags raylib) -o cover main.c cover.c
