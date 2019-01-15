#!/bin/bash
check_files=`find ./scicpp/* -path ./tmp -prune -o \( -name \*.hpp \) -print`

for file in $check_files
do
  cppcheck --enable=performance -I. -Iscicpp "$file"
done
