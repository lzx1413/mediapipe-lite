
#include "mediapipe/framework/api2/builder.h"
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
  api2::builder::Graph graph_cfg;
  auto& node = graph_cfg.AddNode("SquareIntCalculator");
  graph_cfg.In("").SetName("in") >> node.In("");
  node.Out("").SetName("out") >> graph_cfg.Out("");
  auto config = graph_cfg.GetConfig();
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
  #ifdef _WINDOWS
  FLAGS_stderrthreshold = google::GLOG_INFO;
  #else
  FLAGS_stderrthreshold = google::INFO;
  #endif

  FLAGS_colorlogtostderr = true;
  mediapipe::BuildAndRunGraph().ok();
  return 0;
}
