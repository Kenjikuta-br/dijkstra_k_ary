#include <iostream>
#include <sstream>
#include <string>

#include "graph.hpp"

// Aqui estou dando crédito ao meu amigo leo heisler por perceber o erro e ao meu amigo gubavo que me ajudou nessa implementação.
// Usei ela por conveniência para não gastar tanto tempo na parte de leitura do grafo
// no formato dimacs, mudando a implementação do sor levemente para considerar todas
// as arestas, do jeito que estava anteriormente, ele lia os dois comentários após os "p sp" 
// e incrementava o i, fazendo que as duas últimas arestas não fossem lidas.

void Graph::read_dimacs(std::istream& in) {
  // Initialize a string to read each line and a dummy variable for parsing
  std::string line = "", dummy;

  // Read lines from the input stream until we find the header line starting with "p sp"
  while (line.substr(0, 4) != "p sp")
    getline(in, line);  // Read the next line from the input stream

  // Create a stringstream to parse the header line for the number of nodes (n) and edges (m)
  std::stringstream linestr;
  linestr.str(line);  // Assign the header line to the stringstream

  unsigned n;  // Declare variables to store the number of nodes and edges
  // Parse the line: ignore the first two words ("p" and "sp") and extract n (nodes) and m (edges)
  linestr >> dummy >> dummy >> n >> num_edges;

  // Resize the adjacency list to hold 'n' nodes
  resize(n);

  unsigned i = 0;  // Initialize an edge counter

  // Read the remaining lines from the input stream (which contain edge definitions)
  while (getline(in, line) && i < num_edges) {
    // Check if the line starts with "a ", indicating it's an edge definition line
    if (line.substr(0, 2) == "a ") {
      // Create a stringstream to parse the edge line
      std::stringstream arc(line);
      unsigned u, v, w;  // Variables for the edge's start node (u), end node (v), and weight (w)
      char ac;  // Placeholder to discard the first character 'a'
      arc >> ac >> u >> v >> w;  // Parse the edge line (skip 'a' and read u, v, w)

      // Add the edge to the graph using 0-based indexing (subtract 1 from u and v)
      addEdge(u - 1, v - 1, w);
      
      ++i;  // Increment the edge counter
    }
  }
}