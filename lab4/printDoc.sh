#!/bin/sh
echo "============================================================="

for file in $(find .); do
    sed -e '/\/\/+/,/\/\/-/p' -e 'd' $file | sed -e 's/\/\///'
    #sed -e '/\/\/+/,/\/\/-/p' -e 'd' $file | sed -e 's/\/\/-//'
    #sed -e '/\/\/+/,/\/\/-/p' -e 'd' $file | sed -e 's/\/\/+//'
done