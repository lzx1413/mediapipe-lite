# Mediapipe-lite

利用cmake构建的 MediaPipe 图框架核心模块，使用vcpkg进行依赖管理。本项目仅包含核心图引擎（约9M源码），依赖最小化（仅需 abseil, protobuf, glog）。

## 更新日志
* 2026/04/23 清理冗余模块，源码从约45M减少到约9M（减少80%）
* 2023/08/30 支持 [Mac](.github/workflows/macos-x86-cpu-clang.yml), [Linux](.github/workflows/linux-x86-cpu-gcc.yml), [Windows](doc/build_with_msvc.md), [Android](doc/build_with_ndk.md) 编译

## 编译方式

### 方式1: vcpkg (推荐)

vcpkg提供更简单的依赖管理，自动安装所需库。

#### 1. 安装vcpkg
```bash
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
cd ~/vcpkg && ./bootstrap-vcpkg.sh
export VCPKG_ROOT=~/vcpkg  # 添加到 ~/.bashrc 以持久化
```

#### 2. 编译 (GRAPH_ONLY - 最小依赖)
```bash
# 配置 - vcpkg会自动安装依赖 (abseil, protobuf, glog)
cmake -B build -GNinja -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_PROTO_FILES=OFF -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=ON -DBUILD_GRAPH_ONLY=ON

# 编译
cmake --build build -j$(nproc)
```

#### 3. 输出库
- `build/mediapipe/framework/libgraph.a` (~5M) - 核心图引擎
- `build/mediapipe/framework/stream_handler/libstream_handler.a` (~1M) - 流处理器

#### 4. 示例程序
- `build/bin/run_graph` - 基础图示例（PassThroughCalculator）
- `build/bin/new_node` - 自定义计算器示例（SquareIntCalculator）
- `build/bin/new_node_2` - API2 自定义计算器示例
- `build/bin/cpp_graph` - API2 builder 示例

## 工程架构

本项目为最小化构建，仅包含核心图框架：

| 组件 | 大小 | 依赖 | 描述 |
|------|------|------|------|
| **libgraph** | ~5M | abseil, protobuf, glog | 核心图引擎（CalculatorGraph, CalculatorBase, Packet/流管理, 调度器） |
| **libstream_handler** | ~1M | graph | 流处理逻辑（需要whole-archive链接） |
| **calculators/core** | ~300K | - | 12个核心计算器（PassThroughCalculator, MuxCalculator等） |
| **calculators/internal** | - | - | CallbackPacketCalculator（用于测试） |

## CMake编译选项

| 选项 | 默认值 | 描述 |
|------|---------|-------------|
| `BUILD_GRAPH_ONLY` | OFF | 仅编译graph模块 (最小依赖: abseil, protobuf, glog) |
| `BUILD_TESTS` | OFF | 编译单元测试 |
| `BUILD_EXAMPLES` | ON | 编译示例程序 |
| `BUILD_PROTO_FILES` | OFF | 重新生成protobuf文件 |
| `ENABLE_RTTI` | ON | 启用RTTI |

示例: `cmake -B build -DBUILD_TESTS=OFF -DBUILD_GRAPH_ONLY=ON`

## 依赖 (vcpkg)

GRAPH_ONLY模式仅需3个依赖：
- **abseil** - Google C++库
- **protobuf** - Protocol Buffers
- **glog** - Google日志库

## Graph使用示例

具体示例代码见 `examples/tutorial/` 文件夹。

```cpp
// main.cpp
#include "mediapipe/framework/calculator_graph.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

absl::Status PrintHelloWorld() {
    // 配置一个简单的图，连接两个PassThroughCalculator
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
    MP_RETURN_IF_ERROR(graph.Initialize(config)) << "初始化图失败";
    ASSIGN_OR_RETURN(OutputStreamPoller poller, graph.AddOutputStreamPoller("out"));
    MP_RETURN_IF_ERROR(graph.StartRun({}));

    // 发送10个包含"Hello World!"的输入包
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

**注意**：由于mediapipe中的stream_handler和calculator都是通过注册器动态调用的，因此想要在最终可执行文件中确认注册的类可用有两种方式：

1. 将handler和用到的calculator编译成静态库，并通过给ld传参来强制链接所有符号（如上面cmake中对stream_handler库的操作）
2. 直接将handler和calculator相关实现源码与目标文件一起编译

## 运行输出

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

Windows 系统通过MSVC编译请参考 [BuildWithMSVC](./doc/build_with_msvc.md)

## TODO List
* [x] 清理冗余代码（已完成 2026/04/23）
* [ ] 增加 doc, CI, CD, CT, 代码格式化检查等相关流程
* [ ] 完善教程和代码示例
* [ ] 增加pipeline测试benchmark