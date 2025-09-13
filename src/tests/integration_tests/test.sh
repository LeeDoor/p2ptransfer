#!/bin/bash

executable=$1
shift

echo 'y' | "$executable" --get --port 45952 2>&1 > output.txt &
listener_pid=$!

while true; do 
    if grep -q "Connected to " output.txt; then
        break
    fi
    sleep 0.01
done

echo 'test filling' > test_filename
"$executable" --host 127.0.0.1 --port 45952 --filename test_filename
transferer_pid=$!
wait $listener_pid
wait $transferer_pid

rm test_filename

if [[ $listener_running -eq 0 ]] && [[ $transferer_running -eq 0 ]]; then
    exit 0
else
    exit 1
fi
