#include <fmt/core.h>

#include <graph/flow.hpp>
#include <graph/graph.hpp>
#include <graph/push_relabel_graph.hpp>
#include <utility>
#include <utils/assets.hpp>
#include <utils/bench.hpp>

auto main(int argc, char **argv) -> int {
  if (argc < 2) {
    fmt::print("Usage: graphs-flow <ford_fulkerson|push_relabel>\n");
    throw std::invalid_argument("Invalid number of arguments");
  }
  std::string flow_algorithm = argv[1];

  auto elements = utils::assets::get_size_of_assets(fmt::format("{}", utils::bench::filePath));

  if (flow_algorithm == "ford_fulkerson") {
    for (auto element : elements) {
      std::string filename = fmt::format("{}/{}.txt", utils::bench::filePath, element);

      Graph *graph = utils::assets::get_graph_from_file(filename);

      auto [max_flow, total_paths] = flow::ford_fulkerson(*graph, 0, graph->getNumVertices() - 1);

      fmt::print("{}: {} {}\n", element, max_flow, total_paths);
    }
  } else if (flow_algorithm == "push_relabel") {
    for (auto element : elements) {
      std::string filename = fmt::format("{}/{}.txt", utils::bench::filePath, element);

      PushRelabelGraph *graph = utils::assets::get_push_relabel_graph_from_file(filename);

      auto max_flow = graph->getMaxFlow(0, graph->getNumVertices() - 1);

      fmt::print("{}: {}\n", element, max_flow);
    }
  } else {
    fmt::print("Usage: graphs-flow <ford_fulkerson|push_relabel>\n");
    throw std::invalid_argument("Invalid flow algorithm");
  }

  return 0;
}
