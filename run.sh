#!/usr/bin/env bash

FILENAME=$1

scp $FILENAME $PI_HOST:~/tmp/test.s
ssh $PI_HOST 'cd ~/tmp && gcc -o test test.s && echo "---start running---" && ./test'
