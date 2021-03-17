#include <vector>
#ifndef TOURISTSPOTS_H
#define TOURISTSPOTS_H


class Node{
public:
	int x, y, score, idx,  Ti;
	bool visited;
	int tw_open, tw_close  ;//time windows
	 // tempo de Visita

};

class Route{
public:
	int tmax, now_score, now_idx, TI_acumulado, ponto, open_now;
	double  now_time, zero;//time windows
	std::vector <Node> tour;   // Pontos turisticos utilizados // vector da classe Node
	void add_node(Node*, double**);    // * ponteiro   ** ponteiro de ponteiro // https://www.geeksforgeeks.org/double-pointer-pointer-pointer-c/
   // double dist_Acumulado;
	bool timewindows(Node, double**) const;//time windows
	//bool timeorder(Node, double**) const;//time windows



};



#endif
