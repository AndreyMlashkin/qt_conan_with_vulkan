mkdir build -p && \
cd build && \
conan install ../ --build missing && \
cmake -S ../ -B . && \
cmake --build .
cd ..
