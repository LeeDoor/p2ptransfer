cd ../build/$(uname)

if [ -d cli ]; then
    dir=cli
elif [ -d graphics ]; then
    dir=graphics
else
    echo "cant find a directory to run tests here: $(pwd)"
    exit
fi
ctest --test-dir $dir --timeout 2 --output-on-failure --stop-on-failure $@
