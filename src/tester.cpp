#include "dijkstra.hpp"
#include <fstream>
#include <numeric>
#include <random>
#include <chrono>
#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>

class DijkstraTester {
public:
    void runTests(const Graph& graph, int k, int num_runs) {
        // Vector to accumulate statistics for each metric:
        // [0] edges, [1] inserts, [2] extractMins, [3] decreaseKeys, [4] time (µs)
        std::vector<long long> operation_counts(5, 0);
        
        // Vectors to accumulate r values for each operation type
        double insert_r_total = 0.0;
        double extract_min_r_total = 0.0;
        double decrease_key_r_total = 0.0;
        
        int valid_runs = 0;  // Counts runs that produced valid paths
        
        // Random number generation setup for selecting random source-target pairs
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, graph.adj.size() - 1);
        
        // Redirect cout to capture Dijkstra's output
        std::streambuf* old_cout_buffer = std::cout.rdbuf();
        std::ostringstream cout_buffer;
        
        for (int i = 0; i < num_runs; ++i) {
            // Select random source and target (ensuring they're different)
            int source = dis(gen);
            int target = dis(gen);
            while (target == source) target = dis(gen);
            
            Dijkstra dijkstra(graph);
            
            // Redirect cout to our buffer
            std::cout.rdbuf(cout_buffer.rdbuf());
            cout_buffer.str("");  // Clear the buffer
            
            // Run Dijkstra's algorithm (output will be captured in our buffer)
            dijkstra.computeShortestPaths(source, k, true);
            
            // Restore original cout
            std::cout.rdbuf(old_cout_buffer);
            
            // Parse the captured output
            std::string output = cout_buffer.str();
            std::istringstream iss(output);
            std::string token;
            std::vector<double> values;
            
            // Split the output by semicolons
            while (std::getline(iss, token, ';')) {
                if (!token.empty()) {
                    try {
                        values.push_back(std::stod(token));
                    } catch (...) {
                        continue;  // Skip malformed values
                    }
                }
            }
            
            // Expected format: edges;inserts;extracts;decreases;insert_r;extract_r;decrease_r;time
            if (values.size() != 8) {
                continue;  // Skip incomplete runs
            }
            
            // Verify we found a valid path
            int distance = dijkstra.getDistance(target);
            if (distance == std::numeric_limits<int>::max()) {
                continue;  // Skip unreachable targets
            }
            
            // Accumulate statistics
            operation_counts[0] += static_cast<long long>(values[0]);  // edges
            operation_counts[1] += static_cast<long long>(values[1]);  // inserts
            operation_counts[2] += static_cast<long long>(values[2]);  // extractMins
            operation_counts[3] += static_cast<long long>(values[3]);  // decreaseKeys
            operation_counts[4] += static_cast<long long>(values[7]);  // time (µs)
            
            // Accumulate r values
            insert_r_total += values[4];
            extract_min_r_total += values[5];
            decrease_key_r_total += values[6];
            
            valid_runs++;
        }
        
        // Configure output formatting
        std::cout << std::fixed << std::setprecision(6);
        
        if (valid_runs > 0) {
            std::cout << std::fixed 
                << std::setprecision(2) << static_cast<double>((operation_counts[0] / valid_runs)) << ";"
                << std::setprecision(2) << (static_cast<double>(operation_counts[1]) / valid_runs) << ";"
                << std::setprecision(2) << (static_cast<double>(operation_counts[2]) / valid_runs) << ";"
                << std::setprecision(2) << (static_cast<double>(operation_counts[3]) / valid_runs) << ";"
                << std::setprecision(6) << (insert_r_total / valid_runs) << ";"
                << std::setprecision(6) << (extract_min_r_total / valid_runs) << ";"
                << std::setprecision(6) << (decrease_key_r_total / valid_runs) << ";"
                << std::setprecision(2) << (static_cast<double>(operation_counts[4]) / valid_runs) << ";\n";
        } else {
            std::cout << "0;0;0;0;0;0;0;0;\n";
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <k> <num_runs> < graph.txt\n";
        std::cerr << "Example: " << argv[0] << " 5 1000 < graph.txt\n";
        std::cerr << "Where:\n";
        std::cerr << "  <k> - arity of the heap (2 for binary, 4 for 4-ary, etc.)\n";
        std::cerr << "  <num_runs> - number of test runs to perform\n";
        return 1;
    }

    try {
        // Read graph from stdin (redirected from file)
        Graph graph;
        graph.read_dimacs(std::cin);
        
        // Parse command line arguments
        int k = std::stoi(argv[1]);
        int num_runs = std::stoi(argv[2]);

        // Run performance tests
        DijkstraTester tester;
        tester.runTests(graph, k, num_runs);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}