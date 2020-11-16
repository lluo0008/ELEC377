#!/bin/sh
echo "============================================================="

for file in $(find . -type f \( -name '*.c' -o -name '*.h' \)); do
    sed -e '/\/\/+/,/\/\/-/p' -e 'd' $file | sed -e 's/\/\///;s/-//;s/+//'
    #sed -e '/\/\/+/,/\/\/-/p' -e 'd' $file | sed -e 's/\/\/-//'
    #sed -e '/\/\/+/,/\/\/-/p' -e 'd' $file | sed -e 's/\/\/+//'
done