#include <graph/push_relabel_graph.hpp>

PushRelabelGraph::PushRelabelGraph(int V) {
  this->V = V;

  // all vertices are initialized with 0 height
  // and 0 excess flow
  for (int i = 0; i < V; i++) ver.push_back(Vertex(0, 0));
}

void PushRelabelGraph::addEdge(int u, int v, int capacity) {
  // flow is initialized with 0 for all edge
  edge.push_back(PushRelabelEdge(0, capacity, u, v));
}

void PushRelabelGraph::preflow(int s) {
  // Making h of source Vertex equal to no. of vertices
  // Height of other vertices is 0.
  ver[s].h = ver.size();

  //
  for (int i = 0; i < edge.size(); i++) {
    // If current edge goes from source
    if (edge[i].u == s) {
      // Flow is equal to capacity
      edge[i].flow = edge[i].capacity;

      // Initialize excess flow for adjacent v
      ver[edge[i].v].e_flow += edge[i].flow;

      // Add an edge from v to s in residual graph with
      // capacity equal to 0
      edge.push_back(PushRelabelEdge(-edge[i].flow, 0, edge[i].v, s));
    }
  }
}

// returns index of overflowing Vertex
int overFlowVertex(std::vector<Vertex>& ver) {
  for (int i = 1; i < ver.size() - 1; i++) {
    if (ver[i].e_flow > 0) return i;
  }

  // -1 if no overflowing Vertex
  return -1;
}

// Update reverse flow for flow added on ith Edge
void PushRelabelGraph::updateReverseEdgeFlow(int i, int flow) {
  int u = edge[i].v, v = edge[i].u;

  for (int j = 0; j < edge.size(); j++) {
    if (edge[j].v == v && edge[j].u == u) {
      edge[j].flow -= flow;
      return;
    }
  }

  // adding reverse Edge in residual graph
  PushRelabelEdge e = PushRelabelEdge(0, flow, u, v);
  edge.push_back(e);
}

// To push flow from overflowing vertex u
bool PushRelabelGraph::push(int u) {
  // Traverse through all edges to find an adjacent (of u)
  // to which flow can be pushed
  for (int i = 0; i < edge.size(); i++) {
    // Checks u of current edge is same as given
    // overflowing vertex
    if (edge[i].u == u) {
      // if flow is equal to capacity then no push
      // is possible
      if (edge[i].flow == edge[i].capacity) continue;

      // Push is only possible if height of adjacent
      // is smaller than height of overflowing vertex
      if (ver[u].h > ver[edge[i].v].h) {
        // Flow to be pushed is equal to minimum of
        // remaining flow on edge and excess flow.
        int flow = std::min(edge[i].capacity - edge[i].flow, ver[u].e_flow);

        // Reduce excess flow for overflowing vertex
        ver[u].e_flow -= flow;

        // Increase excess flow for adjacent
        ver[edge[i].v].e_flow += flow;

        // Add residual flow (With capacity 0 and negative
        // flow)
        edge[i].flow += flow;

        updateReverseEdgeFlow(i, flow);

        return true;
      }
    }
  }
  return false;
}

// function to relabel vertex u
void PushRelabelGraph::relabel(int u) {
  // Initialize minimum height of an adjacent
  int mh = INT_MAX;

  // Find the adjacent with minimum height
  for (int i = 0; i < edge.size(); i++) {
    if (edge[i].u == u) {
      // if flow is equal to capacity then no
      // relabeling
      if (edge[i].flow == edge[i].capacity) continue;

      // Update minimum height
      if (ver[edge[i].v].h < mh) {
        mh = ver[edge[i].v].h;

        // updating height of u
        ver[u].h = mh + 1;
      }
    }
  }
}

// main function for printing maximum flow of graph
int PushRelabelGraph::getMaxFlow(int s, int t) {
  preflow(s);

  // loop until none of the Vertex is in overflow
  int u = overFlowVertex(ver);
  while (u != -1) {
    bool pushed = push(u);
    if (!pushed) relabel(u);
    u = overFlowVertex(ver);
  }

  // ver.back() returns last Vertex, whose
  // e_flow will be final maximum flow
  return ver.back().e_flow;
}
