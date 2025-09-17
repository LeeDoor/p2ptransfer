#!/bin/bash
executable=$1
shift

echo 'y' | "$executable" --get --port 45952 > output.txt &
listener_pid=$!

while true; do 
    echo 'attempting to grep...'
    if grep -q "Listening at port 45952" output.txt; then
        break
    fi
    sleep 0.1
done

echo 'test filling' > test_filename
"$executable" --host 127.0.0.1 --port 45952 --filename test_filename
wait $listener_pid
last_pid=$?

rm test_filename

if grep -q "File successfully transfered" output.txt; then
    rm output.txt
    if [[ $last_pid -eq 0 ]]; then
        exit 0
    else
        exit 1
    fi
fi
rm output.txt
exit 1
