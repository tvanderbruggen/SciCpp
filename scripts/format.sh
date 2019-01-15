#!/bin/bash
format_files=`find ./*  -type d \( -path ./tmp \) -prune -o \( -name \*.hpp -o -name \*.cpp \) -print`

for file in $format_files
do
  if [ $file != "./tests/catch.hpp" ]; then
    echo $file
    clang-format -i "$file"
  fi
done
