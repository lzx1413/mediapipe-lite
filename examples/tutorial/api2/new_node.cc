// Copyright 2019 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// A simple example to print out "Hello World!" from a MediaPipe graph.

#include "mediapipe/framework/api2/node.h"
#include "mediapipe/framework/calculator_graph.h"
#include "mediapipe/framework/calculator_registry.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {
namespace api2 {
class SquareIntCalculator : public Node {
 public:
  static constexpr Input<int> kIn{""};
  static constexpr Output<int> kOut{""};
  MEDIAPIPE_NODE_CONTRACT(kIn, kOut);
  absl::Status Open(CalculatorContext* cc) final { return absl::OkStatus(); }

  absl::Status Process(CalculatorContext* cc) final {
    int value = *kIn(cc);
    kOut(cc).Send(value * value);
    return absl::OkStatus();
  }
};
MEDIAPIPE_REGISTER_NODE(SquareIntCalculator);
}  // namespace api2

absl::Status BuildAndRunGraph() {
  // Configures a simple graph, which concatenates 2 PassThroughCalculators.
  CalculatorGraphConfig config =
      ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
        input_stream: "in"
        output_stream: "out"
        node {
          calculator: 'SquareIntCalculator'
          input_stream: 'in'
          output_stream: 'out'
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
        "in", MakePacket<int>(i).At(Timestamp(i))));
  }
  // Close the input stream "in".
  MP_RETURN_IF_ERROR(graph.CloseInputStream("in"));
  mediapipe::Packet packet;
  // Get the output packets string.
  while (poller.Next(&packet)) {
    LOG(INFO) << packet.Get<int>();
  }
  return graph.WaitUntilDone();
}
}  // namespace mediapipe

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_stderrthreshold = google::INFO;
  FLAGS_colorlogtostderr = true;
  mediapipe::BuildAndRunGraph().ok();
  return 0;
}