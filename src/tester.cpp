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
        std::vector<long long> counts(4, 0); // insert, extract, decrease, time
        double r_total = 0.0;
        int valid_runs = 0;
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, graph.adj.size() - 1);
        
        std::streambuf* old_cout_buffer = std::cout.rdbuf();
        std::ostringstream cout_buffer;
        
        for (int i = 0; i < num_runs; ++i) {
            int source = dis(gen);
            int target = dis(gen);
            while (target == source) target = dis(gen);
            
            Dijkstra dijkstra(graph);
            
            std::cout.rdbuf(cout_buffer.rdbuf());
            cout_buffer.str("");
            
            dijkstra.computeShortestPaths(source, k, true);
            
            std::cout.rdbuf(old_cout_buffer);
            
            std::string output = cout_buffer.str();
            std::istringstream iss(output);
            std::string token;
            std::vector<double> values;
            
            while (std::getline(iss, token, ';')) {
                if (!token.empty()) {
                    try {
                        values.push_back(std::stod(token));
                    } catch (...) {
                        continue;
                    }
                }
            }
            
            if (values.size() != 5) {
                continue;
            }
            
            int distance = dijkstra.getDistance(target);
            if (distance == std::numeric_limits<int>::max()) {
                continue;
            }
            
            counts[0] += static_cast<long long>(values[0]);
            counts[1] += static_cast<long long>(values[1]);
            counts[2] += static_cast<long long>(values[2]);
            r_total += values[3];
            counts[3] += static_cast<long long>(values[4]);
            
            valid_runs++;
        }
        
        std::cout << std::fixed << std::setprecision(4);
        
        if (valid_runs > 0) {
            std::cout << std::fixed 
                << std::setprecision(2) << (static_cast<double>(counts[0]) / valid_runs) << ";"
                << std::setprecision(2) << (static_cast<double>(counts[1]) / valid_runs) << ";"
                << std::setprecision(2) << (static_cast<double>(counts[2]) / valid_runs) << ";"
                << std::setprecision(6) << (r_total / valid_runs) << ";"
                << std::setprecision(2) << (static_cast<double>(counts[3]) / valid_runs) << ";\n";
        } else {
            std::cout << "0;0;0;0;0;\n";
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <k> <num_runs> < grafo.txt\n";
        std::cerr << "Exemplo: " << argv[0] << " 5 1000 < grafo.txt\n";
        return 1;
    }

    try {
        Graph graph;
        graph.read_dimacs(std::cin);
        int k = std::stoi(argv[1]);
        int num_runs = std::stoi(argv[2]);

        DijkstraTester tester;
        tester.runTests(graph, k, num_runs);

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
        return 1;
    }

    return 0;
}