#include <graph/graph.hpp>
#include <vector>

namespace flow {
  std::pair<int, int> ford_fulkerson(Graph& graph, int begin_vertex, int target_vertex);

  // int push_relabel(Graph& graph, int source, int sink);
}  // namespace flow
