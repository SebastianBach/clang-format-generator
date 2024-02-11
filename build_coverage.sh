rm -rf build/
mkdir build
cd build
mkdir coverage
cmake -DBUILD_COVERAGE=ON ..
cmake --build . --target clean
cmake --build . 
ctest  -VV
gcovr -r .. --html --html-details -o coverage/coverage.html
