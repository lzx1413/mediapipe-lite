# Mediapipe-lite
[![linux-x64-cpu-gcc](https://github.com/lzx1413/mediapipe-lite/actions/workflows/linux-x86-cpu-gcc.yml/badge.svg)](https://github.com/lzx1413/mediapipe-lite/actions/workflows/linux-x86-cpu-gcc.yml)
[![macos-x64-cpu-clang](https://github.com/lzx1413/mediapipe-lite/actions/workflows/macos-x86-cpu-clang.yml/badge.svg)](https://github.com/lzx1413/mediapipe-lite/actions/workflows/macos-x86-cpu-clang.yml)
[![windows-x64-cpu-msvc](https://github.com/lzx1413/mediapipe-lite/actions/workflows/windows-x86-cpu-msvc.yml/badge.svg)](https://github.com/lzx1413/mediapipe-lite/actions/workflows/windows-x86-cpu-msvc.yml)

A lightweight CMake build of Google's MediaPipe graph framework. This project provides only the core graph engine (~9M source code) with minimal dependencies (abseil, protobuf, glog).

## Change Log
* 2026/04/23 Cleaned up redundant modules, reduced source code from ~45M to ~9M (80% reduction)
* 2023/08/30 Supports building on [Mac](.github/workflows/macos-x86-cpu-clang.yml), [Linux](.github/workflows/linux-x86-cpu-gcc.yml), [Windows](doc/build_with_msvc.md), [Android](doc/build_with_ndk.md)

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
cmake -B build -GNinja -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_PROTO_FILES=OFF -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=ON -DBUILD_GRAPH_ONLY=ON

# Build
cmake --build build -j$(nproc)
```

#### 3. Output Libraries
- `build/mediapipe/framework/libgraph.a` (~5M) - Core graph engine
- `build/mediapipe/framework/stream_handler/libstream_handler.a` (~1M) - Stream handlers

#### 4. Example Programs
- `build/bin/run_graph` - Basic graph example with PassThroughCalculator
- `build/bin/new_node` - Custom calculator example (SquareIntCalculator)
- `build/bin/new_node_2` - API2 custom calculator example
- `build/bin/cpp_graph` - API2 builder example

## Project Architecture

This is a minimal build containing only the core graph framework:

| Component | Size | Dependencies | Description |
|-----------|------|--------------|-------------|
| **libgraph** | ~5M | abseil, protobuf, glog | Core graph engine (CalculatorGraph, CalculatorBase, Packet/stream management, Scheduler) |
| **libstream_handler** | ~1M | graph | Stream handling logic (requires whole-archive linking) |
| **calculators/core** | ~300K | - | 12 essential calculators (PassThroughCalculator, MuxCalculator, etc.) |
| **calculators/internal** | - | - | CallbackPacketCalculator for testing |

## CMake Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_GRAPH_ONLY` | OFF | Only build graph module (minimal deps: abseil, protobuf, glog) |
| `BUILD_TESTS` | OFF | Build unit tests |
| `BUILD_EXAMPLES` | ON | Build example programs |
| `BUILD_PROTO_FILES` | OFF | Regenerate protobuf files |
| `ENABLE_RTTI` | ON | Enable RTTI |

Example: `cmake -B build -DBUILD_TESTS=OFF -DBUILD_GRAPH_ONLY=ON`

## Dependencies (vcpkg)

Only 3 dependencies required for GRAPH_ONLY mode:
- **abseil** - Google's C++ library
- **protobuf** - Protocol buffers
- **glog** - Google logging library

## Examples of using Graph

See the `examples/tutorial/` folder for example code.

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
    ASSIGN_OR_RETURN(OutputStreamPoller poller, graph.AddOutputStreamPoller("out"));
    MP_RETURN_IF_ERROR(graph.StartRun({}));

    // Give 10 input packets that contains the same string "Hello World!".
    for (int i = 0; i < 10; ++i) {
        MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(
            "in", MakePacket<std::string>("Hello World!").At(Timestamp(i))));
    }
    MP_RETURN_IF_ERROR(graph.CloseInputStream("in"));

    mediapipe::Packet packet;
    while (poller.Next(&packet)) {
        LOG(INFO) << packet.Get<std::string>();
    }
    return graph.WaitUntilDone();
}
}  // namespace mediapipe

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_stderrthreshold = google::INFO;
    FLAGS_colorlogtostderr = true;
    mediapipe::PrintHelloWorld().ok();
    return 0;
}
```

CMakeLists.txt:

```cmake
add_executable(hello_world hello_world.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc)

if (APPLE)
    target_link_libraries(hello_world PUBLIC graph)
    target_link_libraries(hello_world PUBLIC -Wl,-force_load stream_handler)
else()
    target_link_libraries(hello_world PUBLIC -Wl,--whole-archive stream_handler)
    target_link_libraries(hello_world PUBLIC -Wl,--no-whole-archive graph)
endif()
```

**Note**: Since stream_handler and calculators are called dynamically by registering them with a registrar, there are two ways to make sure the registered classes are available in the final executable:

1. Compile the handler and used calculator into static libraries and force linking of all symbols by passing a parameter to ld (as shown above for stream_handler)
2. Compile the source code of the handler and calculator implementations directly with the target files

## Running Output

```bash
$ build/bin/run_graph
I20260423 21:29:50.520194 run_graph.cc:41] node {
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
I20260423 21:29:50.521051 run_graph.cc:57] Hello World!
I20260423 21:29:50.521085 run_graph.cc:57] Hello World!
...
```

## TODO List
* [x] Clean up redundant code (completed 2026/04/23)
* [ ] Add doc, CI, CD, CT, code formatting checking and other processes.
* [ ] Improve tutorials and code examples
* [ ] Add pipeline test benchmark.