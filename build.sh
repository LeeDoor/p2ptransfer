mkdir build; cd build && cmake -DENABLE_TESTING=ON $@ .. && cmake --build . -j4
