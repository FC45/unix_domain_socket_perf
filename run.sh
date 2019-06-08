#!/bin/bash

echo "ping-pong each 1GB data"

./server > /dev/null 2>&1 &
date +"%T.%N"
./client > /dev/null 2>&1
date +"%T.%N"
