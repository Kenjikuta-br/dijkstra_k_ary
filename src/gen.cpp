/**
 * \file gen.cpp
 *   \author Marcus Ritt <mrpritt@inf.ufrgs.br>
 *   \date Time-stamp: <2024-07-23 13:03:31 ritt>
 *
 *   This program generates a random directed graph with weighted edges,
 *   computes the shortest path between two randomly selected nodes using
 *   Dijkstra’s algorithm, and outputs the graph in DIMACS format.
 */

#include <cassert> // For assert() to validate input arguments
#include <iostream> // For standard input-output operations
using namespace std;

#include <boost/graph/adjacency_list.hpp> // Graph representation
#include <boost/graph/connected_components.hpp> // (Not used in this program)
#include <boost/graph/dijkstra_shortest_paths.hpp> // For Dijkstra's shortest path algorithm
using namespace boost;

// Struct to store information in each vertex (node)
struct VertexInformation {
  unsigned component; // Component number (can be used for connected component analysis)
};

// Struct to store information in each edge
struct EdgeInformation {
  unsigned weight; // Weight (cost) of the edge
};

const unsigned maxweight = 1000; // Maximum possible edge weight

// Define a directed graph using adjacency list representation
typedef adjacency_list<vecS, vecS, directedS, VertexInformation, EdgeInformation> Graph;

// Define graph traits to easily reference nodes (vertices) and edges
typedef graph_traits<Graph>::vertex_descriptor Node; // Node descriptor
typedef graph_traits<Graph>::edge_descriptor Edge; // Edge descriptor

int main(int argc, char *argv[]) {
  // Ensure that exactly two arguments are passed (number of nodes and edge probability)
  assert(argc == 3);

  // Read command-line arguments
  unsigned n = atoi(argv[1]); // Number of vertices
  double p = atof(argv[2]); // Probability of an edge existing between two nodes

  // Initialize random number generator using the current time
  srand48(time(0));

  // (1) Generate a random graph
  Graph g; // Create an empty graph

  // Add `n` vertices to the graph
  for (unsigned i = 0; i < n; i++)
      add_vertex(g);

  // Iterate over all possible pairs of vertices
  for (unsigned i = 0; i < n; i++) {
      for (unsigned j = 0; j < n; j++) {
          if (i != j && drand48() < p) { // Add edge with probability `p`
              Edge e = add_edge(i, j, g).first; // Create a directed edge from `i` to `j`
              g[e].weight = lrand48() % maxweight; // Assign a random weight (0 to maxweight-1)
          }
      }
  }

  // (2) Compute and print an example shortest path
  // Select a random source (starting node) and destination node
  unsigned src = lrand48() % num_vertices(g);
  unsigned dst = lrand48() % num_vertices(g);

  vector<unsigned> dist(n); // Distance array for shortest path computation
  vector<unsigned> pred(n); // Predecessor array to reconstruct the path

  // Run Dijkstra's shortest path algorithm from `src`
  dijkstra_shortest_paths(
      g, src, 
      weight_map(get(&EdgeInformation::weight, g)) // Use edge weights
      .distance_map(&dist[0]) // Store shortest distances here
      .predecessor_map(&pred[0]) // Store path predecessors
  );

  // Output the computed shortest path distance
  cerr << "Distance between " << src + 1 << " and " << dst + 1 
       << " is " << dist[dst] << endl;

  // (3) Print the graph in DIMACS challenge format
  cout << "p sp " << num_vertices(g) << " " << num_edges(g) << endl;

  // Iterate over all edges in the graph and output them in DIMACS format
  for (auto [eb, ee] = edges(g); eb != ee; eb++) {
      cout << "a " << source(*eb, g) + 1 << " " // 1-based index for source
           << target(*eb, g) + 1 << " " // 1-based index for target
           << g[*eb].weight << endl; // Edge weight
  }

  return 0;
}