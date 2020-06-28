#!/bin/bash

printf "\n\n\n\n\n\n\n\n\n\n\n\n\n -------------------- [BUILD] --------------------"
#export SFML="/Users/Mishka/Desktop/SFML-2.5.1"

g++ -c main.cpp -std=c++17 -I"$SFML/include"
g++ main.o -std=c++17 -Wcomment -o main -L"$SFML/lib" -lsfml-graphics -lsfml-window -lsfml-system
printf "\n\n -------------------- [RUN] -------------------- "
# echo $1 : $2 : $3 : $4
export LD_LIBRARY_PATH="$SFML/lib" && export UBSAN_OPTIONS=print_stacktrace=1 && ./main $1
# export LD_LIBRARY_PATH="$SFML/lib" && leaks -atExit -- ./main $1 | grep LEAK: # 
