rm -rf build/
mkdir build
cd build
cmake -DBUILD_GPROF=ON ..
cmake --build . --target clean
cmake --build . 
ctest  -VV
cd src
cd app
gprof clang_format_generator gmon.out > analysis.txt
