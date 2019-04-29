#!/usr/bin/env bash

ISLOCAL=$1

make clean
make mjavac

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

function testparser() {
    FILENAME=$1
    ERRORLINENO=$2
    echo "Testing $FILENAME"
    RESULT=$(./mjavac $FILENAME 2>&1)
    if [[ -z RESULT ]]; then
        rm testcase/parser/*.s
    fi
    if [[ ERRORLINENO -eq -1 ]]; then
        if [[ ! $RESULT =~ ^Syntax ]]; then
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

function testtypechecker() {
    FILENAME=$1
    OUTPUTFILE=$FILENAME.output
    echo "Testing $FILENAME"
    RESULT=$(./mjavac $FILENAME 2>&1)
    if [[ -z RESULT ]]; then
        rm testcase/typechecker/*.s
    fi
    DIFF_RESULT=$(diff <(echo "$RESULT") $OUTPUTFILE)
    if [[ -z $DIFF_RESULT ]]; then
        echo "    ${green}Pass${reset}"
    else
        echo "    ${red}Failed${reset}"
        echo "<<< My Result"
        echo "$RESULT"
        echo ">>> Standard Restuls"
        cat $OUTPUTFILE
        echo ""
        echo "$DIFF_RESULT"
        exit 1
    fi
}

function testinterpretation() {
    FILENAME=$1
    CLASSNAME=$([[ $FILENAME =~ ^(.*)\.java$ ]] && echo ${BASH_REMATCH[1]})
    echo "Testing $FILENAME"
    COMPILE_RESULT=$(../../mjavac $FILENAME 2>&1)
    if [[ -n $COMPILE_RESULT ]]; then
        echo "    ${red}Failed${reset}"
        echo "Compile error"
        echo "$COMPILE_RESULT"
        exit 
    fi
    if [[ $ISLOCAL == "local" ]]; then
        gcc -o test.out ${CLASSNAME}.s
        MY_RESULT=$(./test.out 2>&1)
    else
        scp ${CLASSNAME}.s $PI_HOST:~/tmp/test.s
        ssh $PI_HOST 'cd ~/tmp && gcc -o test test.s'
        MY_RESULT=$(ssh $PI_HOST 'cd ~/tmp && ./test 2>&1')
    fi
    javac $FILENAME
    STD_RESULT=$(java $CLASSNAME)
    rm *.class
    DIFF_RESULT=$(diff <(echo "$MY_RESULT") <(echo "$STD_RESULT"))
    if [[ -z $DIFF_RESULT ]]; then
        rm ${CLASSNAME}.s
        echo "    ${green}Pass${reset}"
    else
        echo "    ${red}Failed${reset}"
        echo "<<< My Result"
        echo "$MY_RESULT"
        echo ">>> Standard Restuls"
        echo "$STD_RESULT"
        echo ""
        echo "$DIFF_RESULT"
        exit 1
    fi
}

echo "--- Testing Parser ---"
for testcase in testcase/parser/*.java; do
    if [[ $testcase =~ \.([0-9]+)\.java$ ]]; then
        testparser $testcase ${BASH_REMATCH[1]}
    else
        testparser $testcase -1
    fi
done

echo ""
echo "--- Testing Typechecker ---"
for testcase in testcase/typechecker/*.java; do
    testtypechecker $testcase
done

echo ""
echo "--- Testing Interpretation---"
cd testcase/interpretation
for testcase in *.java; do
    testinterpretation $testcase
done

