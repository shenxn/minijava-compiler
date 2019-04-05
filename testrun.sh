#!/usr/bin/env bash

make clean && make mjavac
./mjavac $1.java
./run.sh $1.s
