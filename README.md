# Mediapipe-lite
[![linux-x64-cpu-gcc](https://github.com/lzx1413/mediapipe-lite/actions/workflows/linux-x86-cpu-gcc.yml/badge.svg)](https://github.com/lzx1413/mediapipe-lite/actions/workflows/linux-x86-cpu-gcc.yml)
[![macos-x64-cpu-clang](https://github.com/lzx1413/mediapipe-lite/actions/workflows/macos-x86-cpu-clang.yml/badge.svg)](https://github.com/lzx1413/mediapipe-lite/actions/workflows/macos-x86-cpu-clang.yml)
[![windows-x64-cpu-msvc](https://github.com/lzx1413/mediapipe-lite/actions/workflows/windows-x86-cpu-msvc.yml/badge.svg)](https://github.com/lzx1413/mediapipe-lite/actions/workflows/windows-x86-cpu-msvc.yml)

Builds the mediapipe graph module using cmake with vcpkg or conan for dependency management.

## Change Log 
* 2023/08/30 supports building on [Mac](.github/workflows/macos-x86-cpu-clang.yml), [Linux](.github/workflows/linux-x86-cpu-gcc.yml), [Windows](doc/build_with_msvc.md), [Android](doc/build_with_ndk.md) 

## Build Methods

### Method 1: vcpkg (Recommended)

vcpkg provides simpler dependency management with automatic installation.

#### 1. Install vcpkg
```bash
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
cd ~/vcpkg && ./bootstrap-vcpkg.sh
export VCPKG_ROOT=~/vcpkg  # Add to ~/.bashrc for persistence
```

#### 2. Build (GRAPH_ONLY - minimal dependencies)
```bash
# Configure - vcpkg will auto-install dependencies (abseil, protobuf, glog)
cmake --preset release -DBUILD_PROTO_FILES=OFF -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_GRAPH_ONLY=ON

# Build
cmake --build build -j$(nproc)
```

#### 3. Output Libraries
- `build/mediapipe/framework/libgraph.a` (~5M) - Core graph engine
- `build/mediapipe/framework/stream_handler/libstream_handler.a` (~1M) - Stream handlers

### Method 2: Conan + Docker (Legacy)

#### 1. Build the Docker image 
```bash
cd docker 
docker build -t x86_u20_cpp_gcc9 -f Dockerfile.x86_u20_gcc9 --no-cache .
```

#### 2. Start docker
```bash
docker run -v ${PWD}:/mediapipe-lite --name x86_u20_med -it x86_u20_cpp_gcc9:latest -c "cd /mediapipe-lite && bash docker/init_zsh.sh && zsh"
```

#### 3. Compile the test
```bash
# conan install dependencies, run only on first build
conan install . --build=missing -pr:h=docker/x86_gcc_profile
cd build
cmake ..
make -j 4
# Declare the path to the dynamic library, unset to run source deactivate_conanrun.sh
source conanrun.sh
# run unit test
make test
# run object detection example 
# The model in the config file is at https://drive.google.com/file/d/1U9cm5qfOxnGwyB6ypJjYvB6OeOjLZqpC/view?usp=drive_link
# Download and place in mediapipe/models folder
# The test image package can be downloaded from https://drive.google.com/file/d/1IjP8aT_iQ8fV_FCuUJk8TH3_e1X2Y_3Q/view?usp=drive_link
bin/object_detection --calculator_graph_config_file=../examples/desktop/object_detection/object_detection_desktop_live.pbtxt --input_video_path=$IMAGE_DIR --output_video_path=$OUTPUT_DIR
# run object detection example with all verbose logs
GLOG_v=5 bin/object_detection --calculator_graph_config_file=../examples/desktop/object_detection/object_detection_desktop_live.pbtxt --input_video_path=$IMAGE_DIR --output_video_path=$OUTPUT_DIR
```

## Project Architecture
* **libgraph** (~5M) depends only on protobuf, abseil and glog - minimal core graph engine
* **libstream_handler** (~1M) - stream handling logic, requires whole-archive linking
* **libframework** contains libgraph and other auxiliary projects (opencv, eigen, etc.)

## CMake Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_GRAPH_ONLY` | OFF | Only build graph module (minimal deps: abseil, protobuf, glog) |
| `BUILD_TESTS` | ON | Build unit tests |
| `BUILD_EXAMPLES` | ON | Build example programs |
| `BUILD_PROTO_FILES` | ON | Regenerate protobuf files |
| `ENABLE_RTTI` | ON | Enable RTTI |

Example: `cmake --preset release -DBUILD_TESTS=OFF -DBUILD_GRAPH_ONLY=ON`

## Examples of using Graph

See the tutorial folder for example code.

```cpp
    // main.cpp
    #include "mediapipe/framework/calculator_graph.h"
    #include "mediapipe/framework/port/logging.h"
    #include "mediapipe/framework/port/parse_text_proto.h"
    #include "mediapipe/framework/port/status.h"

    namespace mediapipe {

    absl::Status PrintHelloWorld() {
    // Configures a simple graph, which concatenates 2 PassThroughCalculators.
    CalculatorGraphConfig config =
        ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
            input_stream: "in"
            output_stream: "out"
            node {
            calculator: "PassThroughCalculator"
            input_stream: "in"
            output_stream: "out1"
            }
            node {
            calculator: "PassThroughCalculator"
            input_stream: "out1"
            output_stream: "out"
            }
        )pb");
    LOG(INFO) << config.DebugString();
    CalculatorGraph graph;
    MP_RETURN_IF_ERROR(graph.Initialize(config)) << "init graph failed";
    ASSIGN_OR_RETURN(OutputStreamPoller poller,
                    graph.AddOutputStreamPoller("out"));
    MP_RETURN_IF_ERROR(graph.StartRun({}));
    // Give 10 input packets that contains the same string "Hello World!".
    for (int i = 0; i < 10; ++i) {
        MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(
            "in", MakePacket<std::string>("Hello World!").At(Timestamp(i))));
    }
    // Close the input stream "in".
    MP_RETURN_IF_ERROR(graph.CloseInputStream("in"));
    mediapipe::Packet packet;
    // Get the output packets string.
    while (poller.Next(&packet)) {
        LOG(INFO) << packet.Get<std::string>();
    }
    return graph.WaitUntilDone();
    }
    }  // namespace mediapipe

    int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_stderrthreshold=google::INFO;
    FLAGS_colorlogtostderr=true;
    mediapipe::PrintHelloWorld().ok();
    return 0;
    }
    ```
CMakeList.txt

```cmake
add_executable(hello_world hello_world.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc)
target_link_libraries(new_node PUBLIC 
                          -Wl,--whole-archive 
                          stream_handler)
target_link_libraries(hello_world PUBLIC 
                          -Wl,--no-whole-archive 
                        graph)
```

**Note**: Since stream_handler, calculator in mediapipe are called dynamically by registering them with a registrar, there are two ways to make sure the registered classes are available in the final executable.

1. compile the handler, the used calculator into static libraries and force linking of all symbols by passing a parameter to ld, as in the above cmake operation for the stream_handler library
2. compile the source code of the handler and calculator implementations directly with the target files, as in the test_package example.

Running output
```bash
I20230721 17:08:09.523499 140015 hello_world.cc:41] node {
  calculator: "PassThroughCalculator"
  input_stream: "in"
  output_stream: "out1"
}
node {
  calculator: "PassThroughCalculator"
  input_stream: "out1"
  output_stream: "out"
}
input_stream: "in"
output_stream: "out"
I20230721 17:08:09.525810 140015 hello_world.cc:57] Hello World!
I20230721 17:08:09.525838 140015 hello_world.cc:57] Hello World!
I20230721 17:08:09.525846 140015 hello_world.cc:57] Hello World!
I20230721 17:08:09.525852 140015 hello_world.cc:57] Hello World!
I20230721 17:08:09.525859 140015 hello_world.cc:57] Hello World!
I20230721 17:08:09.525866 140015 hello_world.cc:57] Hello World!
I20230721 17:08:09.525872 140015 hello_world.cc:57] Hello World!
I20230721 17:08:09.525879 140015 hello_world.cc:57] Hello World!
I20230721 17:08:09.525885 140015 hello_world.cc:57] Hello World!
I20230721 17:08:09.525892 140015 hello_world.cc:57] Hello World!
```
## TODO List
* [ ] Clean up redundant code step by step
* [ ] Add doc, CI, CD, CT, code formatting checking and other related processes.
* [ ] Improve tutorials and code examples
* [ ] Add pipieline test benchmark.
* [ ] Improve python interface, allow adding python node.