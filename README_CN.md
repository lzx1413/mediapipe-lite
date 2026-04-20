# Mediapipe-lite
利用cmake进行构建mediapipe的graph模块，支持vcpkg或conan进行依赖管理

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
cmake --preset release -DBUILD_PROTO_FILES=OFF -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_GRAPH_ONLY=ON

# 编译
cmake --build build -j$(nproc)
```

#### 3. 输出库
- `build/mediapipe/framework/libgraph.a` (~5M) - 核心图引擎
- `build/mediapipe/framework/stream_handler/libstream_handler.a` (~1M) - 流处理器

### 方式2: Conan + Docker (传统方式)

#### 1. 编译Docker镜像 
```bash
cd docker 
docker build -t x86_u20_cpp_gcc9 -f Dockerfile.x86_u20_gcc9 --no-cache .
```

#### 2. 启动docker
```bash
docker run -v ${PWD}:/mediapipe-lite --name x86_u20_med -it x86_u20_cpp_gcc9:latest -c "cd /mediapipe-lite && bash docker/init_zsh.sh && zsh"
```

#### 3. 编译测试
```bash
# conan安装依赖, 仅在第一次编译时运行
conan install . --build=missing -pr:h=docker/x86_gcc_profile
cd build
cmake ..
make -j 4
# 声明动态库路径, 取消设置可运行 source deactivate_conanrun.sh
source conanrun.sh
# run unit test
make test
# run object detection example 
# config 文件内的模型在 https://drive.google.com/file/d/1U9cm5qfOxnGwyB6ypJjYvB6OeOjLZqpC/view?usp=drive_link
# 下载后放置到 mediapipe/models 文件夹下
# 测试图像包可从 https://drive.google.com/file/d/1IjP8aT_iQ8fV_FCuUJk8TH3_e1X2Y_3Q/view?usp=drive_link 下载
bin/object_detection --calculator_graph_config_file=../mediapipe/graphs/object_detection/object_detection_desktop_live.pbtxt --input_video_path=$IMAGE_DIR --output_video_path=$OUTPUT_DIR
# run object detection example with all verbose logs
GLOG_v=5 bin/object_detection --calculator_graph_config_file=../mediapipe/graphs/object_detection/object_detection_desktop_live.pbtxt --input_video_path=$IMAGE_DIR --output_video_path=$OUTPUT_DIR
```

## 工程架构
* **libgraph** (~5M) 仅依赖 protobuf, abseil 和 glog - 最小核心图引擎
* **libstream_handler** (~1M) - 流处理逻辑，需要whole-archive链接
* **libframework** 包含libgraph和其他辅助工程的整合包 (opencv, eigen等)

## CMake编译选项

| 选项 | 默认值 | 描述 |
|--------|---------|-------------|
| `BUILD_GRAPH_ONLY` | OFF | 仅编译graph模块 (最小依赖: abseil, protobuf, glog) |
| `BUILD_TESTS` | ON | 编译单元测试 |
| `BUILD_EXAMPLES` | ON | 编译示例程序 |
| `BUILD_PROTO_FILES` | ON | 重新生成protobuf文件 |
| `ENABLE_RTTI` | ON | 启用RTTI |

示例: `cmake --preset release -DBUILD_TESTS=OFF -DBUILD_GRAPH_ONLY=ON`

## Graph使用示例

具体示例代码见 tutorial 文件夹

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
**注意**：由于mediapipe中的stream_handler, calculator都是通过注册器注册来动态调用的，因此想要在最终可执行文件中确认注册的类可用有两种方式

1. 将handler, 用到的calculator 编译成静态库，并通过给ld传参来强行链接所有符号，如上面cmake中对stream_handler库的操作
2. 直接将handler， calculator相关实现源码，与目标文件一起编译，如test_package中的例子

运行输出
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
Windows 系统通过MSVC编译本工程请参考 [BuildWithMSVC](./doc/build_with_msvc.md)

## TODO List
* [ ] 逐步清理冗余代码
* [ ] 增加 doc, CI, CD, CT, 代码格式化检查等相关流程
* [ ] 完善教程和代码示例
* [ ] 增加pipeline测试benchmark
* [ ] 完善python接口，允许增加python node
