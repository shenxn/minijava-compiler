#!/usr/bin/env bash

make clean
make parser

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

function test() {
    FILENAME=$1
    ERRORLINENO=$2
    echo "Testing $FILENAME"
    RESULT=`./parser $FILENAME`
    if [[ ERRORLINENO -eq -1 ]]; then
        if [[ -z $RESULT ]]; then
            echo "    ${green}Pass${reset}"
        else
            echo "    ${red}Failed${reset}"
            echo "    No error is expected. Got"
            echo "    $RESULT"
            exit 1
        fi
    else
        if [[ $RESULT == "Syntax errors in $ERRORLINENO" ]]; then
            echo "    ${green}Pass${reset}"
        else
            echo "    ${red}Failed${reset}"
            echo "    Errors expected at $ERRORLINENO. Got"
            echo "    $RESULT"
            exit 1
        fi
    fi
}

for testcase in testcase/*; do
    if [[ $testcase =~ \.([0-9]+)\.java$ ]]; then
        test $testcase ${BASH_REMATCH[1]}
    else
        test $testcase -1
    fi
done
