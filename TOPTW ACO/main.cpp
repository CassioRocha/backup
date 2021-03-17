#include <iostream>
#include <ctime>
#include <vector>
// #include <stdlib.h>// atoi; cast into int

#include "graph.h"
#include "touristSpots.h"
#include "colony.h"
#include "fileloader.h"


int main(int argc, char *argv[]){
	if (argc < 2){
		std::cerr << "argc: " << argc << "should be >= 2";
		return -1;
	}
	const clock_t start_time = clock();
	std::vector<std::vector<int> > param;
	int route_capacity, m_routes;
	route_capacity=0;
	m_routes=0;
	file_loader(argv[1], param, route_capacity, m_routes);
	
	// Graph* graph = new Graph(param, route_capacity, num_route);
	// graph->run_GreedyAlgorithm();
	// graph->show_each_route_tour();
	// graph->show_node_info();
	// graph->show_distance_matrix();
	// delete graph;
	// graph = nullptr;
	
	Colony* colony = new Colony(param, route_capacity, m_routes);
	colony->run_ACO();
	// colony->show_distance_matrix();
	delete colony;
	colony = nullptr;

	const clock_t end_time = clock();
	std::cout << (end_time - start_time)/ double(CLOCKS_PER_SEC) << "sec" << std::endl;
	return 0;
}
