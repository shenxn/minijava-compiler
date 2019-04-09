#!/usr/bin/env bash

./mjavac $1.java
scp $1.s $PI_HOST:~/tmp/test.s
ssh $PI_HOST 'cd ~/tmp && gcc -o test test.s && echo "---start running---" && ./test'
