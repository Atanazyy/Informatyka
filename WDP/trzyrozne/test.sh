#!/usr/bin/env bash

    g++ -fsanitize=undefined,address -Wall -Wextra -pedantic -Wshadow -Wfloat-equal -Wformat=2 -std=c++17 -o trzgen.e trzgen.cpp
    gcc @opcje trz.c -o trz.e

    echo "GOTOWE";

for(( i=1;i>0;i++)) do
echo $i | ./trzgen.e > wejscie.in
./three_diffrent-master/trz.e < wejscie.in > brut.out
./trz.e < wejscie.in > wzor.out

if diff -qb brut.out wzor.out;
    then echo -ne "test $i           OK\r";
    else echo "test $i          BOMBA W BOJLERZE";
        break;
fi
done
