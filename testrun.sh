#!/usr/bin/env bash

make clean && make mjavac
./run.sh $1
