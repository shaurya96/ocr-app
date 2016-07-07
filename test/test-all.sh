#!/bin/bash

set -e

test_count=0
divider='\n================================================================\n'
error_divider=`tr '=' '>' <<< "$divider"`

negative_tests=""

for dir in *; do
    if [ -d "$dir" ] && [ $dir != makefiles ]; then
        if [[ $dir == bad* ]]; then
            negative_tests+="$dir "
            continue
        fi
        printf $divider
        printf "> Running test %s\n\n" $dir
        pushd $dir
        make -f Makefile.x86 clean run WORKLOAD_ARGS=""
        popd
        test_count=$((test_count+1))
    fi
done

for dir in $negative_tests; do
    printf $divider
    printf "> Running negative test %s\n\n" $dir
    pushd $dir
    ! make -f Makefile.x86 clean run WORKLOAD_ARGS="" 2>&1 | tee raw_output.tmp
    tr -d '\r\n' < raw_output.tmp | sed -f check.sed > filtered_output.tmp
    if ! [ -s filtered_output.tmp ]; then
        printf $error_divider
        printf "> ERROR: Missing expected output from failure test.\n\n"
        exit 1
    fi
    popd
    test_count=$((test_count+1))
done

printf $divider
printf "> All %d tests passed!\n\n" $test_count