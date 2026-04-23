# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

mediapipe-lite is a CMake/vcpkg build of Google's MediaPipe graph framework. It enables building graph-based processing pipelines for tasks like object detection, image processing, and video analysis. The project supports cross-platform deployment (Linux, macOS, Windows, Android) using vcpkg for dependency management.

## Build Commands

### Initial Setup (vcpkg)
```bash
# Install vcpkg if not already installed
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
cd ~/vcpkg && ./bootstrap-vcpkg.sh

# Set environment variable (add to ~/.bashrc for persistence)
export VCPKG_ROOT=~/vcpkg
```

### Build (GRAPH_ONLY - minimal dependencies)
```bash
# Configure - vcpkg will auto-install dependencies (abseil, protobuf, glog)
cmake --preset release -DBUILD_PROTO_FILES=OFF -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_GRAPH_ONLY=ON

# Build
cmake --build build -j$(nproc)
```

### Output Libraries
- `build/mediapipe/framework/libgraph.a` (~5M) - Core graph engine
- `build/mediapipe/framework/stream_handler/libstream_handler.a` (~1M) - Stream handlers

### Regenerate Protobuf Files
If you need to regenerate protobuf files after dependency changes:
```bash
PROTOC=build/vcpkg_installed/x64-linux/tools/protobuf/protoc
find mediapipe -name "*.proto" | while read proto; do
  cc_file="${proto%.proto}.pb.cc"
  h_file="${proto%.proto}.pb.h"
  rm -f "$cc_file" "$h_file"
  $PROTOC --proto_path=. --cpp_out=. "$proto"
done
```

### Code Formatting
```bash
cmake --build build --target clang-format
```

## CMake Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_SHARED_LIBS` | OFF | Build as shared library |
| `BUILD_TESTS` | ON | Build unit tests |
| `BUILD_EXAMPLES` | ON | Build example programs |
| `BUILD_GRAPH_ONLY` | OFF | Only build graph module (minimal deps) |
| `WITH_GPU` | OFF | Support GPU runtime |
| `BUILD_PYTHON` | OFF | Build Python bindings |
| `ENABLE_PROFILER` | OFF | Enable graph profiler |
| `ENABLE_RTTI` | ON | Enable RTTI |
| `BUILD_PROTO_FILES` | ON | Regenerate protobuf files during cmake configure |

Example: `cmake --preset release -DBUILD_TESTS=OFF -DBUILD_GRAPH_ONLY=ON`

## Architecture

### Core Libraries

1. **libgraph** (~5M): Minimal core graph engine. Dependencies: protobuf, abseil, glog. Contains:
   - `CalculatorGraph` - Main graph orchestration
   - `CalculatorBase` - Base class for all calculators
   - Packet/stream management
   - Scheduler and executor

2. **libstream_handler** (~1M): Stream handling logic. Must be linked with `--whole-archive` to ensure registration symbols are loaded.

### Key Components

- **Calculators**: Processing nodes in the graph. Implement `CalculatorBase`, register via `REGISTER_CALCULATOR(MyCalculator)` macro. Located in `mediapipe/calculators/`.

- **Stream Handlers**: Control packet flow between calculators. Located in `mediapipe/framework/stream_handler/`.

- **Graph Config**: Defined via protobuf text format (`.pbtxt` files). Specifies nodes, input/output streams, and connections.

### Calculator Registration Pattern

Calculators and stream handlers use dynamic registration. Two approaches to ensure symbols are linked:

1. **Whole-archive linking** (recommended for static libs):
```cmake
target_link_libraries(target PUBLIC -Wl,--whole-archive stream_handler -Wl,--no-whole-archive graph)
```

2. **Direct source compilation**: Compile calculator source directly with target.

### Calculator Implementation Pattern

```cpp
class MyCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    cc->Inputs().Get(0).Set<MyType>();
    cc->Outputs().Get(0).Set<MyType>();
    return absl::OkStatus();
  }

  absl::Status Open(CalculatorContext* cc) override { return absl::OkStatus(); }
  absl::Status Process(CalculatorContext* cc) override {
    cc->Outputs().Get(0).AddPacket(...);
    return absl::OkStatus();
  }
};
REGISTER_CALCULATOR(MyCalculator);
```

### Running a Graph

```cpp
CalculatorGraphConfig config = ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
  input_stream: "in"
  output_stream: "out"
  node { calculator: "PassThroughCalculator" input_stream: "in" output_stream: "out" }
)pb");

CalculatorGraph graph;
graph.Initialize(config);
auto poller = graph.AddOutputStreamPoller("out").value();
graph.StartRun({});
graph.AddPacketToInputStream("in", MakePacket<std::string>("data").At(Timestamp(0)));
graph.CloseInputStream("in");
Packet packet;
while (poller.Next(&packet)) { /* handle output */ }
graph.WaitUntilDone();
```

## Platform-Specific Notes

### Windows (MSVC)
- Requires Visual Studio 2022, CMake 3.15+
- vcpkg integrates automatically via toolchain file
- Whole-archive: `/WHOLEARCHIVE:stream_handler`

### Linux
- Requires GCC 6+ or Clang 5+
- Install `curl zip unzip tar` for vcpkg bootstrap
- Uses Ninja generator (install via `pip install ninja`)

### Android NDK
See `doc/build_with_ndk.md` for Android build instructions.

## Verbose Logging

For debug output: `GLOG_v=5 bin/example --calculator_graph_config_file=...`

## Code Style

Google style (`.clang-format` based on Google). Run clang-format target before commits.

## Dependencies (vcpkg)

Managed via `vcpkg.json` manifest. Core dependencies:
- **abseil** - Google's C++ library
- **protobuf** - Protocol buffers
- **glog** - Google logging library

### vcpkg Target Names

vcpkg uses component-based targets:
- `absl::strings`, `absl::status`, `absl::synchronization`, etc. (not `abseil::abseil`)
- `protobuf::libprotobuf` (not `protobuf::protobuf`)
- `glog::glog`

The CMakeLists.txt creates aliases for compatibility with existing code that uses `abseil::abseil` and `protobuf::protobuf`.