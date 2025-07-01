mkdir win_build; cd win_build && cmake -DENABLE_TESTING=OFF -DCMAKE_TOOLCHAIN_FILE=../toolchain/mingw.cmake .. && cmake --build . -j16

