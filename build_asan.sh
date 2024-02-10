rm -rf build/
mkdir build
cd build
mkdir coverage
cmake -DBUILD_ASAN=ON ..
cmake --build . --target clean
cmake --build . 
ctest  -VV

