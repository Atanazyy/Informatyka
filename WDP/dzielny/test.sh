#/!/bin/bash

gcc @opcje dzielny.c -o dzielny.e

for((i=0;i<=15;i++))
{
    ./dzielny.e < "./testy/dzielny$i.in" > p.out
    if diff -wbc p.out "./testy/dzielny$i.out" > log.out
    then
        echo "$i OK"
    else
        echo "$i NIE OK"
        break
    fi
}
