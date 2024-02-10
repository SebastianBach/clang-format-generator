rm -rf build/
mkdir build
cd build
mkdir coverage
cmake -DBUILD_UBSAN=ON ..
cmake --build . --target clean
cmake --build . 
ctest  -VV

