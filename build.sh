#!/bin/bash

clang $(pkg-config --libs --cflags raylib) main.c cover.c
