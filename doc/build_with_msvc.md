## Build with MSVC

1. Compilation environment:
   1. Windows 11
   2. Visual Studio 2022

2. Compilation steps:
   1. Install vcpkg
    ```bash
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg && bootstrap-vcpkg.bat
    set VCPKG_ROOT=<vcpkg install dir>
    ```
   2. Configure and build
    ```bash
    cmake --preset release -DBUILD_PROTO_FILES=OFF -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_GRAPH_ONLY=ON
    cmake --build build --config Release
    ```
   3. Run unit tests (if BUILD_TESTS=ON)
    ```bash
    cd build
    ctest -V
    ```
