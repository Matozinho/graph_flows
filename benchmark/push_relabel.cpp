#include <benchmark/benchmark.h>
#include <fmt/core.h>

#include <graph/push_relabel_graph.hpp>
#include <utils/assets.hpp>
#include <utils/bench.hpp>

static void BM_Push_Relable(benchmark::State& state) {
  auto memoEntries = utils::assets::memo(utils::assets::get_push_relabel_graph_from_file);

  std::string filename = fmt::format("{}/{}.txt", utils::bench::filePath, state.range(0));
  PushRelabelGraph* g = memoEntries(filename);

  for (auto _ : state) {
    g->getMaxFlow(0, g->getNumVertices() - 1);
  }
}
