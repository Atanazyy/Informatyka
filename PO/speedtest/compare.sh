#!/bin/bash
dir=$(pwd)
echo "Środowisko:"
uname -mrs
echo "Wersja Javy:"
java --version
echo "Czas Javy dla n = 1e8 przy wywołaniu bez dodatkowych flag:"
time java primes.java > /dev/null
echo "Czas Javy dla n = 1e8 przy wywołaniu z -Xcomp -Xdiag:"
time java -Xcomp -Xdiag primes.java > /dev/null
echo "Czas Javy dla n = 1e8 przy wywołaniu z -Xint -Xdiag:"
time java -Xint -Xdiag primes.java > /dev/null
echo "Wersja Node.js:"
node --version
echo "Czas Javascriptu dla n = 1e8:"
time node primes.js > /dev/null
echo "Wersja g++:"
g++ --version
echo "Czas kompilacji c++:"
time g++ primes.cpp
echo "Czas wykonania c++ dla n = 1e8:"
time ./a.out > /dev/null
rm a.out
cp primes.rock ~/rockstar/satriani
cd ~/rockstar/satriani
echo "Czas Rockstara dla n = 1e5:"
time node rockstar primes.rock > /dev/null
rm primes.rock

