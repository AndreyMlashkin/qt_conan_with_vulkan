mkdir build -p && \
cd build && \
conan install ../ && \
cmake -S ../ -B . && \
cmake --build .
cd ..
