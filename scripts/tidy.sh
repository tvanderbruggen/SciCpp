#!/bin/bash
tidy_files=`find ./scicpp/* -path ./tmp -prune -o \( -name \*.hpp \) -print`
checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,hicpp-*,performance-*,modernize-*,readability-*,portability-*,cert-*,misc-*

for file in $tidy_files
do
  clang-tidy "$file" -checks=$checks -- -I. -I./scicpp -std=c++17 
done
