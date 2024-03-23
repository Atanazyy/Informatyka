#!/bin/bash
dir=$(pwd)
echo "Środowisko:"
uname -mrs
echo "Wersja Javy:"
java --version
echo "Czas Javy dla n = 1e8 przy wywołaniu bez flag:"
/usr/bin/time -f "%e" java primes.java
echo "Czas Javy dla n = 1e8 przy wywołaniu z -Xcomp -Xdiag:"
/usr/bin/time -f "%e" java -Xcomp -Xdiag primes.java
echo "Czas Javy dla n = 1e8 przy wywołaniu z -Xint -Xdiag:"
/usr/bin/time -f "%e" java -Xint -Xdiag primes.java
echo "Wersja Node.js:"
node --version
echo "Czas Javascriptu dla n = 1e8:"
/usr/bin/time -f "%e" node primes.js
echo "Wersja g++:"
g++ --version | sed -n '1 p'
echo "Czas kompilacji C++ bez flag:"
/usr/bin/time -f "%e" g++ primes.cpp
echo "Czas wykonania C++ dla n = 1e8 bez flag:"
/usr/bin/time -f "%e" ./a.out
echo "Czas kompilacji C++ z O3:"
/usr/bin/time -f "%e" g++ primes.cpp -O3
echo "Czas wykonania C++ dla n = 1e8 z O3:"
/usr/bin/time -f "%e" ./a.out
rm a.out
cp primes.rock ~/rockstar/satriani
cd ~/rockstar/satriani
echo "Czas Rockstara dla n = 1e5:"
/usr/bin/time -f "%e" node rockstar primes.rock > /dev/null
rm primes.rock

