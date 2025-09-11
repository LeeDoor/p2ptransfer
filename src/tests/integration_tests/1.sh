#!/bin/bash

SHOULD_FAIL="true"

executable=$1
shift

$executable --get

exit_code=$?

if [ "$SHOULD_FAIL" = "true" ]; then
    # Ожидаем ошибку (любой код кроме 0)
    [ $exit_code -ne 0 ] && exit 0 || exit 1
else
    # Ожидаем успех (код 0) или "command not found" (код 127)
    [ $exit_code -eq 0 ] || [ $exit_code -eq 127 ] && exit 0 || exit 1
fi
