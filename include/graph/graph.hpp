#pragma once

#include <stdexcept>
#include <vector>

class Edge {
public:
  int target;
  int capacity;
  int usedCapacity;

  Edge(int capacity, int target) {
    this->capacity = capacity;
    this->usedCapacity = 0;
    this->target = target;
  }
};

class Graph {
private:
  int V;                                // Number of vertices
  std::vector<std::vector<Edge*>> adj;  // Adjacency list

public:
  Graph(int vertices) {
    V = vertices;
    adj.resize(V);
  }

  // Add an edge to the graph
  void addEdge(int u, int v, int capacity) {
    if (u < 0 || u >= V || v < 0 || v >= V) {
      throw std::invalid_argument("Invalid vertex");
    }

    Edge* edge = new Edge(capacity, v);
    adj[u].push_back(edge);
  }

  // Get the number of vertices in the graph
  int getNumVertices() { return V; }

  // Get the adjacency list of a vertex
  std::vector<Edge*>& getAdjacencyList(int v) { return adj[v]; }

  ~Graph() {
    // Clean up dynamically allocated memory
    for (int i = 0; i < V; i++) {
      for (Edge* edge : adj[i]) {
        delete edge;
      }
    }
  }
};
