#include <fmt/core.h>

#include <graph/graph.hpp>
#include <utils/assets.hpp>

auto main() -> int {
  Graph *g = utils::assets::get_graph_from_file("assets/10.txt");

  fmt::print("Number of vertices: {}\n", g->getNumVertices());

  // iterate over the adjacency list of all vertices
  for (int i = 0; i < g->getNumVertices(); i++) {
    fmt::print("Adjacency list of vertex {}: {} elements\n", i, g->getAdjacencyList(i).size());
    for (Edge *edge : g->getAdjacencyList(i)) {
      fmt::print("Target: {} | Capacity: {}\n", edge->target, edge->capacity);
    }
  }

  return 0;
}
