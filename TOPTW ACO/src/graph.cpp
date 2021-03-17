#include <iostream>
#include <vector>
#include <string>
#include <cmath> // sqrt, pow
#include <iomanip> // std::setprecision
#include "graph.h"

using namespace std;


void Graph::calc_node_distance()
{
for(int i = 0; i < num_node; i++){
			  // a distância do hotel até o  primeiro ponto é "0",  já que a janela de tempo só começa a contar a partir do primeiro ponto visita                                                  
     	
	//	 distance_matrix[0][i] = 0;

		 //distance_matrix[i][0] = 0.0;

	//	   cout <<"distac i " << 0 << " -  j "<< i<< "  "<<distance_matrix[0][i] << endl;
	  //     cout << "distac j"<< i << " -  i "<<  0<< "  "<<distance_matrix[i][0] << endl;

	for(int j = i + 1; j < num_node; j++){
		distance_matrix[j][i]=  distance_matrix[i][j]  = sqrt(pow(nodes[i].x - nodes[j].x, 2) + pow(nodes[i].y - nodes[j].y, 2));
	
//		 distance_matrix[0][j] = 0.0;

	     

		}
	}

}
Graph::Graph(std::vector<std::vector<int> > param, int route_TMX , int m_routes)
{
	this->m_routes = m_routes;
	this->num_node = param.size(); 

//	std::cout << "constructor!" << std::endl;
	std::cout << "DIA:" << m_routes << " route:" \
<< route_TMX << " num_node(Hotel + Pontos turisticos):" << num_node << std::endl;

	nodes = new Node[num_node];
	routes = new Route[m_routes];

	for(int i = 0; i < num_node; i++){
		nodes[i].idx = param[i][0]; // i100ndice
		nodes[i].x = param[i][1]; // coord X
		nodes[i].y = param[i][2];  // coord y
		nodes[i].score = param[i][4]; // score
		nodes[i].visited = false;   // Visitas
		
		//time windows
		nodes[i].tw_open = param[i][8]; // abertura da janela de tempo
		nodes[i].tw_close = param[i][9]; // fechamento da janela de tempo
	    nodes[i].Ti = param[i][3];   // Tempo Máximo por cada ponto

	}
	
		tmax = route_TMX; //recebendo o Tmax
	for(int i = 0; i < m_routes; i++){
		
		routes[i].tmax = route_TMX;
		routes[i].now_score = 0; // score inicial
		routes[i].now_idx = 0; 
		routes[i].TI_acumulado=0;
		routes[i].now_time = 0;
	}
	distance_matrix = new double* [num_node]; 
	for(int i = 0; i < num_node; i++){
		distance_matrix[i] = new double [num_node];
		for(int j = 0; j < num_node; j++){
			distance_matrix[i][j] = 0;// initialization all elements in a row
		}
	}
	calc_node_distance();
}


Graph::~Graph()
{ 
	std::cout << "destruido!" << std::endl;
	delete[] nodes;
	nodes = nullptr;
	delete[] routes;
	routes = nullptr;
	for(int i = 0; i < num_node; i++){
		delete[] distance_matrix[i];
		distance_matrix[i] = nullptr;
	}
	delete[] distance_matrix;
	distance_matrix = nullptr;
}

void Graph::show_distance_matrix() const
{
	//std::cout << num_node << "*" << num_node << " distance matrix." << std::endl;
	for(int i = 0; i < num_node; i++){
		for(int j = 0; j < num_node; j++){
			std::cout <<std::fixed << std::setprecision(1) << distance_matrix[i][j]  << " ";
		}
		std::cout << std::endl;
	}
}

bool Graph::visited_node() const
{
	for(int i = 1; i < num_node; i++){
		if(!nodes[i].visited){
			return false;
		}
	}
	return true;
}

void Graph::run_GreedyAlgorithm() // guloso
{
	int route_idx = 0;
	
	while(!visited_node()){ // verificar os nós não visitados
		int best_node_idx;
		double min_distance = 100000;
		double scoret = 0;
		
		
		bool is_return_depot = true;
		
		if(routes[route_idx].tour.empty()){  // empty testa se está vazio
			routes[route_idx].add_node(&nodes[0], distance_matrix);// nodes[0] is hotel
		}


		for(int node_idx = 1; node_idx < num_node; node_idx++){
			  int score= nodes[node_idx].score;
   cout <<" score iiiiiiiiiiii " <<score << endl;   
   
			if(!nodes[node_idx].visited){
		//			if(routes[route_idx].timeorder(nodes[node_idx], distance_matrix)){
					if(routes[route_idx].timewindows(nodes[node_idx], distance_matrix)){
						
						double tmp_distance = distance_matrix[routes[route_idx].now_idx][node_idx];
                   
						if( score >  scoret){
							
							scoret =score ;
							is_return_depot = false;
							best_node_idx = node_idx;
						}
			//		}
					}
			}
			
		}

		if(!is_return_depot){
			routes[route_idx].add_node(&nodes[best_node_idx], distance_matrix);
		}
		else{// if edges are not found, return depot
			if(route_idx + 1 < m_routes){// check if the rest of vehicles exists
				if(routes[route_idx].now_idx != 0){// in case the vehicle did not return back to the depot
					routes[route_idx].add_node(&nodes[0], distance_matrix);
				}
				route_idx += 1;// assign next vehicle
			}
			else{
				std::cout << "Cannot solve this by Greedy algorithm." << std::endl;
				break;// std::exit(0);
			}
			
		}
	}//while loop done
	if(routes[route_idx].now_idx != 0){// in case the vehicle did not return back to the depot
				routes[route_idx].add_node(&nodes[0], distance_matrix);
	}
	std::cout << "algorithm done." << std::endl;
}

void Graph::calc_tour_distance(std::vector<Node>tour, double &tour_distance, double &scoreatual) const
{
	if(tour.size() > 2){


		for(int j = 0; j < tour.size() - 1; j++){
			tour_distance += distance_matrix[tour[j].idx][tour[j+1].idx];
			 scoreatual  += tour[j].score;
			 cout<<"Point : " <<tour[j].idx <<"  distância Acumulada " << tour_distance << endl;
             
		}

	}

}

void Graph::show_each_route_tour() const
{
	double scoreacumulado=0;
	double total_tour_distance = 0.0;
	int total_visited_customer = 0;
	for(int i = 0; i < m_routes; i++){
		if(routes[i].tour.size() > 1){// Imprimi rotas que como tamanho maior que um  
			std::cout << "route da número " << i+1 << " tour: "; // "i" imprimi a rota que está sendo utilizada
			double tour_distance = 0.0;
			  double      scoreatual =0.0;

			for(int j = 0; j < routes[i].tour.size(); j++){  // imprimi os pontos pertencente a rota "i"
 				std::cout << routes[i].tour[j].idx << " ";
			}
			calc_tour_distance(routes[i].tour, tour_distance, scoreatual );
			total_tour_distance += tour_distance;

			std::cout << " " << std::fixed << std::setprecision(1) << tour_distance << " Time ";
			std::cout << ", visited customer:" << routes[i].tour.size()-2;  
	    	std::cout << ", now_time:" << routes[i].now_time;
			std::cout << ", Score:" << routes[i].now_score  << std::endl;
			scoreacumulado = scoreacumulado  + routes[i].now_score;

			scoreatual +=  scoreacumulado;

			total_visited_customer += routes[i].tour.size()-2;
//std::cout << ", \n\n" << std::endl;
	std::cout << "total visited customer:" << total_visited_customer << "/" << num_node - 1 << std::endl;// -1 denotes not counting the depot
	std::cout << "total distance:" << total_tour_distance << "km" << std::endl;
	std::cout << "Score Total:" << scoreacumulado << std::endl;
		}
		else{
		//	std::cout << "Route" << i << ": not used" << std::endl;
		}
	}


}

void Graph::show_node_info() const
{
	std::cout << std::endl << "idx,x,y,score,tw_open,tw_close" << std::endl;
	for(int i = 0; i < num_node; i++){
		std::cout << nodes[i].idx << " " << nodes[i].x << " " << nodes[i].y << " " << nodes[i].score << \
			" " << nodes[i].tw_open << " " << nodes[i].tw_close << " "  << std::endl;
	}
}
