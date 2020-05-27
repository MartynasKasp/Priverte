#!/bin/bash

bison -d -v priverte.y
flex priverte.l
g++ -std=c++11 tree.cpp priverte.tab.c lex.yy.c -o priverte
./priverte in.priverte