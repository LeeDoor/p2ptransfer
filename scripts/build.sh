# Help printing
if [[ "$1" == "--help" ]];
then
    echo 'USAGE: ./build.sh [<0=CLI|1=GRAPHICS|2=ALL(default)>] [build directory name (i.e. windows, linux)] [CMake params...]'
    exit 0
fi

./check_requirements.py

# Reading view mode parameter
graphics=${1:-2}
shift

# if build mode unspecified run both
if [[ $graphics -eq 2 ]];
then
    ./build.sh 0 $@ && ./build.sh 1 $@
    exit
fi

cd ../

# Reading build and source directories
src=$(pwd)
build_dir=${1:-$(uname)}
shift

# Creating build directory
mkdir build/$build_dir -p
cd build/$build_dir

# Printing logs
highlight='\033[0;36m'
clearcolor='\033[0m'
printf "${highlight}building ${src} at ${build_dir} with: "
if [[ $graphics -eq 1 ]];
then
    echo "graphics"
else
    echo "cli"
fi
printf "${clearcolor}"

# Executing CMake
sudo cmake $src -DTESTING=OFF -DGRAPHICS=$graphics $@ && cmake --build . -j$(nproc)
