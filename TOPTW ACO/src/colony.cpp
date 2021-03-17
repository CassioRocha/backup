// Observação a função de de verificar a capacidade do veiculo
// pode ser usada para verificar 

#include <iostream>
#include <vector>
#include <random> // rand_dev
#include <algorithm> // std::max_element
#include "colony.h"
using namespace std;
// https://qiita.com/ganariya/items/25824f1502478a673005	
const double Colony::init_phero = 1.0;// initilaize pheromone
const double Colony::evap_rate = 0.9;// evaporate when pheromone updates
const double Colony::alpha = 3.0;// prob_next_point 
const double Colony::beta = 5.0;// prob_next_point 
const double Colony::q = 1.0;// divided by tour length
double Colony::threshold = 0.5;// whether choose random or follow phero
const double Colony::decay_rate = 0.95;	
  double INFIN = 10000;



void Colony::run_ACO()
{
	double min_distance = INFIN;
	int scorebefore= 0.0;

	
	for(int i = 0; i < ant_time_step; i++){  // número de interações 
		std::cout << std::endl << "time step: " << i << std::endl; 
		move_ants();
		double total_tour_distance = 0.0;
		int scorenow = 0.0;
		update_phero_matrix(total_tour_distance,scorenow);
		if ( scorenow >= scorebefore  ){  // 
	//cout <<  "scoreeee"<< scorebefore <<"min_distance"<<min_distance<<endl ;


			min_distance = total_tour_distance; // 
//cout << "min_distance" << min_distance<< endl;
			scorebefore  = scorenow;
	//		cout << "scorenow" << scorenow<< endl;

		///	min_distance -= scorenow  ;
	//	cout << "min_distance depois" << min_distance<< endl;


			show_each_route_tour();
		}
		update_threshold();

		reset_at_each_step();
	}
}

void Colony::reset_at_each_step()
{
	for (int i = 0; i < m_routes; i++){
		routes[i].tour.clear();		
		routes[i].now_score = 0; // score
		routes[i].now_idx = 0; 
		routes[i].now_time = 0;
	}
	for (int i = 0; i < num_node; i++){
		nodes[i].visited = false;  // seta todos os pontos iniciamente como visita falsa
	}
}

void Colony::update_threshold()
{
	threshold *= decay_rate;
}

void Colony::update_phero_matrix(double &total_tour_distance,int &total_tour_score )
{
	for (int i = 0; i < m_routes; i++){ // m_routes total de dias ou seja rotas
		double tour_distance = 0;
		double scoreatual = 0;
		calc_tour_distance(routes[i].tour, tour_distance,  scoreatual);
		total_tour_distance += tour_distance;
		total_tour_score += scoreatual;
	//	cout << total_tour_score<< " total_tour_score"<< endl;
		for (int j = 0; j < routes[i].tour.size(); j++){
			if(j+1 != routes[i].tour.size()){
				phero_matrix[routes[i].tour[j].idx][routes[i].tour[j+1].idx] = evap_rate * phero_matrix[routes[i].tour[j].idx][routes[i].tour[j+1].idx] + q / tour_distance;
			} 
		}
	}
}

void Colony::move_ants()
{
int route_idx = 0;

	while(!visited_node()){  // número de formigas por rotas // tempo atual  < Graph::tmax 
		if(routes[route_idx].tour.empty()){
		   routes[route_idx].add_node(&nodes[0], distance_matrix);// nodes[0] denotes depot 
		}
	std::vector< std::pair<int, int> > feasible_edges; 
	std::vector< std::pair<int, int> > feasible_score;
	feasible_edges.clear();
	feasible_score.clear();
		while (feasible_edges.empty() && routes[route_idx].now_time  <= Graph::tmax ){ // TMAX
			for(int node_idx = 1; node_idx < num_node; node_idx++){
				if(!nodes[node_idx].visited){
		//				if(routes[route_idx].timeorder(nodes[node_idx], distance_matrix)){
						if(routes[route_idx].timewindows(nodes[node_idx], distance_matrix)){
							
							feasible_edges.push_back(std::make_pair(routes[route_idx].now_idx, node_idx));
							feasible_score.push_back(std::make_pair(nodes[route_idx].score, node_idx));
		//				}
				}
				}
			}
			if(feasible_edges.empty()){
				routes[route_idx].now_time += 1;;// wait for the tw_open at the depot
			}
		
	}	
	if(feasible_edges.empty()){// if still edges are not found, return depot
		if(route_idx + 1 < m_routes){// check if the rest of vehicles exists
			if(routes[route_idx].now_idx != 0){// caso não retorne ao hotel
		 		routes[route_idx].add_node(&nodes[0], distance_matrix);
			}
			route_idx += 1;// assign next vehicle			
		}
		else{
		//	std::cout << "cannot visit all customers this step." << std::endl;
			break;
		}
	}
	else{
	int next_node_idx;
			if (get_rand() < threshold){// choose randomly next node, this prevents local optimization
				next_node_idx = feasible_edges[rand() % feasible_edges.size()].second;
			}
			else{// follow heuristc and pheromone formula, choose next node
				std::vector<double> ups, probs, cumulative_sum;
				double sum = 0.0;
				for (auto feasible_edge : feasible_edges){
				double up = calc_prob(feasible_edge.first, feasible_edge.second);
				//	double up = calc_prob_score( feasible_edge.second);

					sum += up;
					ups.push_back(up);
				}
				for (auto up : ups){
					probs.push_back(up / sum);
				}
				cumulative_sum.push_back(probs.front());
				for (int i = 0; i < probs.size() - 1; i ++){
					probs[i+1] += probs[i];
					cumulative_sum.push_back(probs[i+1]);
				}
				int candi_idx, best_idx;
				double candi_v;
				double best_v = INFIN;
				double r = get_rand();
				for (int x = 0; x < cumulative_sum.size(); x++){
					if (r <= cumulative_sum[x]){
						candi_idx = x;
						candi_v = cumulative_sum[x];
						if (candi_v < best_v){
							best_idx = candi_idx;
							best_v = candi_v;
						}
					}
				}
				if (best_v == INFIN){
						break;
				}
				next_node_idx = feasible_edges[best_idx].second;
				
				// search algorithm
				// double rand = get_rand();
				// // std::sort(cumulative_sum.begin(), cumulative_sum.end());
				// for (auto itr = cumulative_sum.begin(); itr != cumulative_sum.end(); itr++){
				// 	if (rand_num < *itr){
				// 		int next_node_idx = itr;
				// 		break;
				// 	}
				// }
				// auto iter = std::max_element(cumulative_sum.begin(), cumulative_sum.end());
				// next_node_idx = std::distance(cumulative_sum.begin(), iter);// https://teramonagi.hatenablog.com/entry/20130225/1361793892
			}
			routes[route_idx].add_node(&nodes[next_node_idx], distance_matrix);
		}
	}// while loop done
	if(routes[route_idx].now_idx != 0){// caso não retorne ao deposito forçar o retorno
		routes[route_idx].add_node(&nodes[0], distance_matrix); // adiciona o hotel como ponto final
	}
}



double Colony::calc_prob(int now_node_idx, int next_node_idx) const
{
	double ETAij = pow (etha_matrix[now_node_idx][next_node_idx], beta); // distância


	// ETAij = pow ( score[next_node_idx], beta); // distância

	double TAUij = pow (phero_matrix[now_node_idx][next_node_idx], alpha);// pheromônio
	return ETAij * TAUij;
}



/*
double Colony::calc_prob_score( int next_node_idx) const
{
	double ETAij =  (etha_matrix[next_node_idx], beta); // distância
	// ETAij = pow ( score[next_node_idx], beta); // distância

	double TAUij =  (phero_matrix[next_node_idx], alpha);// pheromônio
	return ETAij * TAUij;
}
*/


double Colony::get_rand() const
{
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<double> distr(0, 1);
    return distr(generator);
}

Colony::Colony(std::vector<std::vector<int> > param,
				int route_capacity, int m_routes) : Graph(param, route_capacity, m_routes)
{
	std::cout << "constructor!!" << std::endl;
	phero_matrix = new double* [num_node];
	etha_matrix = new double* [num_node]; 
	for(int i = 0; i < num_node; i++){
		phero_matrix[i] = new double [num_node];
		etha_matrix[i] = new double [num_node];
		for(int j = 0; j < num_node; j++){
			phero_matrix[i][j] = 0;// initialize all elements in a row
			etha_matrix[i][j] = 0;
		}
	}
	for(int i = 0; i < num_node; i++){
		for(int j = i + 1; j < num_node; j++){
			phero_matrix[i][j] = phero_matrix[j][i] = init_phero;
			etha_matrix[i][j] = etha_matrix[j][i] = q / distance_matrix[i][j]   ;
		}
	}
}

Colony::~Colony()
{ 
	std::cout << "destructor!!" << std::endl;
	for(int i = 0; i < num_node; i++){
		delete[] phero_matrix[i];
		phero_matrix[i] = nullptr;
		delete[] etha_matrix[i];
		etha_matrix[i] = nullptr;
	}
	delete[] phero_matrix;
	phero_matrix = nullptr;
	delete[] etha_matrix;
	etha_matrix = nullptr;
}
