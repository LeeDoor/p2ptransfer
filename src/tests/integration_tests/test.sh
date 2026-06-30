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
listener_res=$?

rm test_filename

if grep -q "File successfully transfered" output.txt; then
    rm output.txt
    printf "listener returned with $listener_res"
    exit $listener_res
fi

printf "\033[0;31m\"File successfully transfered\" line is not found in output.txt:\033[0m\n"
cat output.txt
exit 1
