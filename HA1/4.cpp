/* Topological sort of graph.

Expects a list of vertices in a file "vertices.txt"
Format: \d+\s\d+
e.g.:
1 1
1 15
15 15
2 2

Outputs original vertices numbers in decreasing order
(the lower the number is, the lower its order number is)
*/

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <stdexcept>
#include <algorithm> // for std::find
#include <unordered_set>

// expect a list of vertices in a file "vertices.txt"
// format: \d+\s\d+
// e.g.:
// 1 1
// 1 15
// 15 15
// 2 2

// outputs original vertices numbers in decreasing order
// (the lower the number is, the lower its order number is)

size_t verticesNumber = 0;
std::vector<unsigned int> lookedVertices;

void readEdges(std::vector<std::array<unsigned int, 2>> &edgesVector, std::string filename)
{
	std::ifstream infile(filename);
	std::unordered_set<unsigned int> vertices;
	unsigned int a, b;

	while(infile >> a >> b) {
		vertices.insert(a);
		vertices.insert(b);
		edgesVector.push_back({a, b});
	}

	verticesNumber = vertices.size();
}

bool looked(unsigned int vertex)
{
	return std::find(lookedVertices.begin(), lookedVertices.end(), vertex) != lookedVertices.end();
}

unsigned int findMin(std::vector<std::array<unsigned int, 2>> &edges)
{
	bool flag = true;

	for(const auto& edge : edges) {
		if(looked(edge[0])) continue;
		for(const auto& e : edges) {
			// check there are no edges with this one on the right
			// except for reflecsive
			if((e[1] == edge[0]) && (e[0] != e[1])) {
				flag = false;
				break;
			}

			flag = true;
		}

		if(flag) {
			return edge[0];
		}
	}
	throw std::runtime_error("Could not find minimum element");
}

void processVertex(std::vector<std::array<unsigned int, 2>> &edges, unsigned int vertex)
{
	// almost DFS
	for(const auto& edge : edges) {
		if((edge[0] == vertex) && (edge[0] != edge[1]) && !looked(edge[1])) { // pick a connected vertex
			processVertex(edges, edge[1]);
		}
	}

	lookedVertices.push_back(vertex);
}


int main(int argc, char** argv)
{
	try {
		std::vector<std::array<unsigned int, 2>> edges;
		readEdges(edges, "vetrices.txt");

		while(lookedVertices.size() != verticesNumber) {
			processVertex(edges, findMin(edges));
		}

		for(const auto a : lookedVertices) {
			std::cout << a << std::endl;
		}

	    return 0;
	}
	catch(const std::exception& e) {
		std::cerr << "Topological sort: " << e.what() << std::endl;
		return 1;
	}
}