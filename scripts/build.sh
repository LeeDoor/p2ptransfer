if [ $# -eq 0 ]
then
    echo 'USAGE: ./build.sh <0=CLI|1=GRAPHICS>'
    exit 0
fi

graphics=$1

cd ../

src=$(pwd)

mkdir build/x86_64_linux/ -p
cd build/x86_64_linux/

if [ $graphics -eq 1 ]
then
    mkdir graphics/ -p
    cd graphics/
else
    mkdir cli/ -p
    cd cli/
fi
cmake $src -DTESTING=ON -DGRAPHICS=$graphics && cmake --build . -j4
