testing=OFF
graphics=ON

mkdir ../build; 
cd ../build && cmake .. -DTESTING=$testing -DGRAPHICS=$graphics $@ && cmake --build . -j4
