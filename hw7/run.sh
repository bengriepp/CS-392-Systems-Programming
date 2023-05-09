#!/bin/bash
PORT=8080

gcc -o server server.c
gcc -o client client.c

./server $PORT &
./client $PORT
