#!/bin/sh

if [ $# -ne 1 ]
then 
    echo "Usage $0 directoryname."
    exit 1
fi 

mainCheck= false
modCheck= false

if [ -d "$1" ]; then
    echo "Main Files: "
    for file in 'find "$1" -type f -name '*.c''; do
        if grep -q "int main" $file
            then
                printfNum="'grep -c printf $file'"
                fprintfNum="'grep -c fprintf $file'"
                mainCheck=true
                echo "$file $printfNum $fprintfNum" | awk '{printf "%s: %d,%d\n", $1, $2, $3}'
        fi
    done  
    if [ "$mainCheck" = false ]
        then 
            echo "No Main Files"
    fi

    echo "Module Files: "
    for file in 'find "$1" -type f -name '*.c''; do
        if grep -q "init_module" $file
            then
                printkline='grep -n printk $file | sed -e 's/:.*$//' '
                modCheck= true
                echo "$file" | awk '{printf "%s: ", $1}'
                echo $printkline | tr ' ' ','
        fi
    done
    if [ "$modCheck" = false ]
        then
            echo "No Module Files"
    fi
else
    echo "The first argument is not a directory."
    exit 1
fi
        


