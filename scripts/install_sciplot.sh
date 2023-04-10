#!/bin/bash

TMP_DIR=/tmp/sciplot

rm -rf ${TMP_DIR}
git clone https://github.com/sciplot/sciplot --recursive ${TMP_DIR}
cp -r ${TMP_DIR}/sciplot /usr/include
