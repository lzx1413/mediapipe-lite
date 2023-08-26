Compilation environment:
1. windows 11
2. visual studio 2022

Compilation steps:
1. Install miniconda
2. Install cmake, delete the FindProtobuf.cmake file in the C:\Program Files\CMake\share\cmake-3.27\Modules folder, otherwise it will conflict with protobuf under conan
3. Install conan in the conda environment
    ```bash 
    pip install conan
    conan profile detect --force
    # set compiler.cppstd=17 in ~/.conan2/profiles/default
    ```
4. Use conan to install dependencies
    ```bash 
    mkdir build
    conan install . --build=missing 
    ```
5. Compile
    ```bash
    cd build
    cmake ..
    cmake --build . --config Release 
    ```
    Or use visual studio to open the mediapipelite.sln in the build folder for compilation
6. Run unit tests
    ```bash
    ctest -V
    ```