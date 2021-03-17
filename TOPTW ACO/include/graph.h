#include <vector>
#include <unordered_map>
#include "touristSpots.h"
#ifndef GRAPH_H
#define GRAPH_H
using namespace std;

class Graph{
public:
	// std::vector<route> routes;
	// std::vector<Node> nodes;
	// std::vector<std::vector<double>> distance_matrix;
	int m_routes, num_node;
	Route* routes;
	Node* nodes;
	double** distance_matrix;
	void calc_node_distance();
	void calc_tour_distance(std::vector<Node>, double&, double&) const;
	Graph(std::vector<std::vector<int> >, int, int);

	int recebeDistancia;
	int tmax;
	virtual ~Graph();
	void show_distance_matrix() const;
	bool visited_node() const;
	void run_GreedyAlgorithm();
	void show_each_route_tour() const;
	void show_node_info() const;
};

#endif