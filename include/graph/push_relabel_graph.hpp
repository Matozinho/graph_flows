#pragma once
// C++ program to implement push-relabel algorithm for
// getting maximum flow of graph
#include <bits/stdc++.h>

#include "fmt/core.h"

struct PushRelabelEdge {
  // To store current flow and capacity of edge
  int flow, capacity;

  // An edge u--->v has start vertex as u and end
  // vertex as v.
  int u, v;

  PushRelabelEdge(int flow, int capacity, int u, int v) {
    this->flow = flow;
    this->capacity = capacity;
    this->u = u;
    this->v = v;
  }
};

// Represent a Vertex
struct Vertex {
  int h, e_flow;

  Vertex(int h, int e_flow) {
    this->h = h;
    this->e_flow = e_flow;
  }
};

// To represent a flow network
class PushRelabelGraph {
  int V;  // No. of vertices
  std::vector<Vertex> ver;
  std::vector<PushRelabelEdge> edge;

  // Function to push excess flow from u
  bool push(int u);

  // Function to relabel a vertex u
  void relabel(int u);

  // This function is called to initialize
  // preflow
  void preflow(int s);

  // Function to reverse edge
  void updateReverseEdgeFlow(int i, int flow);

public:
  PushRelabelGraph(int V);  // Constructor

  // function to add an edge to graph
  void addEdge(int u, int v, int w);

  // returns maximum flow from s to t
  int getMaxFlow(int s, int t);
};
