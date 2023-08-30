## Build with NDK for android 

1. setup ndk
   * download and unzip android ndk package (we have tested with ndk v22b)
   * set 'export ANDROID_NDK_HOME=Your NDK root dir'  to your .bashrc or .zshrc
2. cross-build 
   * generate protobuf cpp files with your host profile(Linux, Mac or Windows)
   * `cp mediapipe/framework/stream_handler/default_input_stream_handler.pb.cc test_package`
   * `conan create  . -pr docker/ndk_profile --build=missing`
3. test
   * copy the generated 'example' file in test_package to you android deivce's '/data/local/tmp' 
   * just run it and the results will be like bellow:
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
    I20230831 13:39:44.532290 16470 example.cc:55] 64
    I20230831 13:39:44.532302 16470 example.cc:55] 81
    ```