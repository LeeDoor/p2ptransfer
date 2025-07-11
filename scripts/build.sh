# Help printing
if [[ "$1" == "--help" ]];
then
    echo 'USAGE: ./build.sh [<0=CLI|1=GRAPHICS>] [build directory name (i.e. windows, linux)] [CMake params...]'
    exit 0
fi

# Reading view mode parameter
graphics=${1:-0}
shift

cd ../

# Reading build and source directories
src=$(pwd)
build_dir=${1:-$(uname)}
shift

# Creating build directory
mkdir build/$build_dir -p
cd build/$build_dir

# Printing logs
echo -n "building $src at $build_dir with: "
if [[ $graphics -eq 1 ]];
then
    mkdir graphics/ -p
    cd graphics/
    echo "graphics"
else
    mkdir cli/ -p
    cd cli/
    echo "cli"
fi

# Executing CMake
cmake $src -DTESTING=ON -DGRAPHICS=$graphics $@ && cmake --build . -j4
