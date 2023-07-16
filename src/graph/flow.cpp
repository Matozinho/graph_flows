#include <fmt/core.h>

#include <algorithm>
#include <graph/flow.hpp>
#include <graph/graph.hpp>
#include <iostream>
#include <limits>
#include <queue>
#include <stack>
#include <utility>
#include <vector>

namespace flow {
  bool bfs(Graph& graph, int s, int t, std::vector<int>& parent) {
    int V = graph.getNumVertices();
    std::vector<bool> visited(V, false);
    std::queue<int> q;

    visited[s] = true;
    q.push(s);
    parent[s] = -1;

    while (!q.empty()) {
      int u = q.front();
      q.pop();

      for (Edge* edge : graph.getAdjacencyList(u)) {
        int v = edge->target;

        if (!visited[v] && edge->capacity - edge->usedCapacity > 0) {
          visited[v] = true;
          q.push(v);
          parent[v] = u;

          if (v == t) {
            return true;
          }
        }
      }
    }

    return false;
  }

  std::pair<int, int> ford_fulkerson(Graph& graph, int begin_vertex, int target_vertex) {
    int maxFlow = 0;
    int paths = 0;

    while (true) {
      std::vector<int> parent(graph.getNumVertices(), -1);

      if (!bfs(graph, begin_vertex, target_vertex, parent)) {
        break;
      }

      int pathFlow = std::numeric_limits<int>::max();
      // fmt::print("========Path {}========\n", paths);

      // Find the minimum residual capacity along the path
      for (int v = target_vertex; v != begin_vertex; v = parent[v]) {
        int u = parent[v];
        // fmt::print("passing through edge {} -> {}\n", u, v);
        for (Edge* edge : graph.getAdjacencyList(u)) {
          if (edge->target == v) {
            int residualCapacity = edge->capacity - edge->usedCapacity;
            pathFlow = std::min(pathFlow, residualCapacity);
            break;
          }
        }
      }

      // fmt::print("flow: {}\n", pathFlow);

      // Update the flow and residual capacities along the path
      for (int v = target_vertex; v != begin_vertex; v = parent[v]) {
        int u = parent[v];
        for (Edge* edge : graph.getAdjacencyList(u)) {
          if (edge->target == v) {
            edge->usedCapacity += pathFlow;
            break;
          }
        }

        // Subtract the path flow from the reverse edge (if present)
        for (Edge* edge : graph.getAdjacencyList(v)) {
          if (edge->target == u) {
            edge->usedCapacity -= pathFlow;
            break;
          }
        }
      }

      paths++;
      maxFlow += pathFlow;
    }

    return std::make_pair(maxFlow, paths);
  }

  void print_vector(std::vector<int> vec) {
    for (int i = 0; i < vec.size(); i++) std::cout << vec[i] << " ";
    std::cout << std::endl;
  }

  void print_queue(std::queue<int> q) {
    while (!q.empty()) {
      std::cout << q.front() << " ";
      q.pop();
    }
    std::cout << std::endl;
  }

  void print_stack(std::stack<int> s) {
    while (!s.empty()) {
      std::cout << s.top() << " ";
      s.pop();
    }
    std::cout << std::endl;
  }

  int get_minimun_height_of_neighbors(Graph& graph, int u, std::vector<int>& height) {
    int min_height = std::numeric_limits<int>::max();

    for (Edge* edge : graph.getAdjacencyList(u)) {
      if (edge->capacity - edge->usedCapacity > 0) {
        min_height = std::min(min_height, height[edge->target]);
      }
    }

    return min_height;
  }

  void preflow(Graph& graph, std::vector<int>* height, std::vector<int>* excess, int source) {
    (*height)[source] = graph.getNumVertices();
    // (*excess)[source] = std::numeric_limits<int>::max();

    for (Edge* edge : graph.getAdjacencyList(source)) {
      edge->usedCapacity = edge->capacity;
      (*excess)[edge->target] = edge->capacity;
    }
  }

  int push_relabel(Graph& graph, int source, int sink) {
    int V = graph.getNumVertices();

    // Initialize height and excess flow of each vertex
    std::vector<int> height(V, 0);
    std::vector<int> excess(V, 0);
    std::vector<bool> visited(V, false);
    std::stack<int> q;

    preflow(graph, &height, &excess, source);

    // Push-relabel algorithm
    q.push(source);
    // add all adjacent vertices to the queue of the source vertex
    for (Edge* edge : graph.getAdjacencyList(source)) {
      q.push(edge->target);
    }

    visited[source] = true;

    while (!q.empty()) {
      fmt::print("===============Vertex {}===============\n", q.top());
      fmt::print("Queue: ");
      print_stack(q);

      int u = q.top();
      q.pop();

      for (Edge* edge : graph.getAdjacencyList(u)) {
        // fmt::print("edge: {} -> {} | capacity: {} | usedCapacity: {}\n", u, edge->target,
        //            edge->capacity, edge->usedCapacity);

        if (edge->capacity - edge->usedCapacity > 0) {
          if (height[u] > height[edge->target]) {
            int flow = std::min(excess[u], edge->capacity - edge->usedCapacity);
            edge->usedCapacity += flow;

            for (Edge* reverse_edge : graph.getAdjacencyList(edge->target)) {
              if (reverse_edge->target == u) {
                reverse_edge->usedCapacity -= flow;
                break;
              }
            }

            excess[u] -= flow;
            excess[edge->target] += flow;

            if (!visited[edge->target]) {
              q.push(edge->target);
              visited[edge->target] = true;
            }
          }
        }
      }

      if (u != source && u != sink && excess[u] > 0) {
        int minimun_height = get_minimun_height_of_neighbors(graph, u, height);

        if (minimun_height != std::numeric_limits<int>::max()) {
          height[u] = minimun_height + 1;
          q.push(u);
        }
      }

      fmt::print("        s A B C D F t\n");

      fmt::print("height: ");
      print_vector(height);

      fmt::print("excess: ");
      print_vector(excess);
    }

    return excess[sink];
  }
}  // namespace flow
