#include <benchmark/benchmark.h>
#include <fmt/core.h>

#include <graph/flow.hpp>
#include <graph/graph.hpp>
#include <utils/assets.hpp>
#include <utils/bench.hpp>

static void BM_Ford_Fulkerson(benchmark::State& state) {
  auto memoEntries = utils::assets::memo(utils::assets::get_graph_from_file);

  std::string filename = fmt::format("{}/{}.txt", utils::bench::filePath, state.range(0));
  Graph* g = memoEntries(filename);

  for (auto _ : state) {
    flow::ford_fulkerson(*g, 0, g->getNumVertices() - 1);
  }
}
