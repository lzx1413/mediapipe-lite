## Build with NDK for android

1. setup ndk
   * download and unzip android ndk package (we have tested with ndk v22b)
   * set 'export ANDROID_NDK_HOME=Your NDK root dir'  to your .bashrc or .zshrc
2. cross-build
   * generate protobuf cpp files with your host profile(Linux, Mac or Windows)
   * `cp mediapipe/framework/stream_handler/default_input_stream_handler.pb.cc test_package`
   * configure and build with vcpkg and NDK toolchain:
    ```bash
    cmake --preset release -DBUILD_PROTO_FILES=OFF -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_GRAPH_ONLY=ON \
      -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
      -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
      -DANDROID_ABI=arm64-v8a
    cmake --build build -j$(nproc)
    ```
3. test
   * copy the generated 'example' file in test_package to you android device's '/data/local/tmp'
   * just run it and the results will be like below:
    ``` bash
    c2q:/data/local/tmp $ ./example
    I20230831 13:39:44.527252 16470 example.cc:39] node {
    calculator: "SquareIntCalculator"
    input_stream: "in"
    output_stream: "out"
    }
    input_stream: "in"
    output_stream: "out"
    I20230831 13:39:44.532116 16470 example.cc:55] 0
    I20230831 13:39:44.532189 16470 example.cc:55] 1
    I20230831 13:39:44.532212 16470 example.cc:55] 4
    I20230831 13:39:44.532227 16470 example.cc:55] 9
    I20230831 13:39:44.532239 16470 example.cc:55] 16
    I20230831 13:39:44.532253 16470 example.cc:55] 25
    I20230831 13:39:44.532266 16470 example.cc:55] 36
    I20230831 13:39:44.532279 16470 example.cc:55] 49
    I20230831 13:39:44.532302 16470 example.cc:55] 64
    I20230831 13:39:44.532312 16470 example.cc:55] 81
    ```
