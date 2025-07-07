if [ $# -eq 0 ]
then
    echo 'USAGE: ./build.sh <0=CLI|1=GRAPHICS>'
    exit 0
fi

graphics=$1
shift

cd ../

src=$(pwd)
build_dir=${1:-x86_64_linux}
echo $build_dir
shift

mkdir build/$build_dir -p
cd build/$build_dir

if [ $graphics -eq 1 ]
then
    mkdir graphics/ -p
    cd graphics/
else
    mkdir cli/ -p
    cd cli/
fi
cmake $src -DTESTING=ON -DGRAPHICS=$graphics $@ && cmake --build . -j4
