#include <fmt/core.h>

#include <graph/flow.hpp>
#include <graph/graph.hpp>
#include <graph/push_relabel_graph.hpp>
#include <utility>
#include <utils/assets.hpp>

void populate_graph(Graph& graph) {
  graph.addEdge(0, 1, 3);
  graph.addEdge(0, 2, 3);
  graph.addEdge(0, 3, 4);

  graph.addEdge(1, 4, 2);

  graph.addEdge(2, 1, 10);
  graph.addEdge(2, 4, 1);

  graph.addEdge(3, 5, 5);

  graph.addEdge(4, 3, 1);
  graph.addEdge(4, 5, 1);
  graph.addEdge(4, 6, 2);

  graph.addEdge(5, 6, 5);
}

auto main() -> int {
  // Graph graph(8);
  // populate_graph(graph);
  // Graph graph2(8);
  // populate_graph(graph2);
  //
  // int source = 0;
  // int sink = 7;
  //
  // fmt::print("===========Starting BFS==========\n");
  // std::pair<int, int> result = flow::fordFulkerson(graph, source, sink);
  //
  // int maxFlow = result.first;
  // int foundPaths = result.second;

  // print the used capacity of each edge
  // fmt::print("BFS max flow: {} | with {} paths\n", maxFlow, foundPaths);
  // for (int i = 0; i < graph.getNumVertices(); i++) {
  //   for (Edge* edge : graph.getAdjacencyList(i)) {
  //     fmt::print("{} --> {}: Remaining capacity: {}\n", i, edge->target,
  //                edge->capacity - edge->usedCapacity);
  //   }
  // }

  // Graph* fordFulkerson = utils::assets::get_graph_from_file("assets/test/1.txt");
  // PushRelabelGraph* pushRelabel = utils::assets::get_graph_from_file("assets/test/1.txt");

  // std::pair<int, int> result
  //     = flow::fordFulkerson(*fordFulkerson, 0, fordFulkerson->getNumVertices() - 1);

  // int pushRelabelResult = flow::push_relabel(*pushRelabel, 0, pushRelabel->getNumVertices() - 1);

  // int maxFlow = result.first;
  // int foundPaths = result.second;

  // fmt::print("BFS max flow: {} | with {} paths\n", maxFlow, foundPaths);
  // fmt::print("Push-Relabel max flow: {}\n", pushRelabelResult);

  int V = 7;
  PushRelabelGraph push_relabel_graph(V);
  // Graph graph(7);
  // populate_graph(graph);

  // int max_flow = flow::push_relabel(graph, 0, graph.getNumVertices() - 1);
  // fmt::print("Maximum flow is {}\n", max_flow);
  //
  // // iterate over the adjacency list of all vertices
  // for (int i = 0; i < graph.getNumVertices(); i++) {
  //   fmt::print("=========================\n");
  //   for (Edge* edge : graph.getAdjacencyList(i)) {
  //     fmt::print("{} --> {} | Capacity: {} | usedCapacity: {}\n", i, edge->target,
  //     edge->capacity,
  //                edge->usedCapacity);
  //   }
  // }

  // Creating above shown flow network
  push_relabel_graph.addEdge(0, 1, 3);
  push_relabel_graph.addEdge(0, 2, 3);
  push_relabel_graph.addEdge(0, 3, 4);

  push_relabel_graph.addEdge(1, 4, 2);

  push_relabel_graph.addEdge(2, 1, 10);
  push_relabel_graph.addEdge(2, 4, 1);

  push_relabel_graph.addEdge(3, 5, 5);

  push_relabel_graph.addEdge(4, 3, 1);
  push_relabel_graph.addEdge(4, 5, 1);
  push_relabel_graph.addEdge(4, 6, 2);

  push_relabel_graph.addEdge(5, 6, 5);

  // Initialize source and sink
  int s = 0, t = 5;

  fmt::print("Maximum flow is {}\n", push_relabel_graph.getMaxFlow(s, t));

  return 0;
}
