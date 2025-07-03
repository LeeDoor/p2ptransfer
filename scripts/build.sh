if [[ "$#" -le 0 || "$#" -ge 3 ]]; then
    echo 'USAGE: ./build.sh <0=CLI|1=GRAPHICS> [<0=NO TESTS|1=TESTS>]'
    exit 0
fi

graphics=$1
testing=${2:-ON}

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
cmake $src -DTESTING=$testing -DGRAPHICS=$graphics && cmake --build . -j4
