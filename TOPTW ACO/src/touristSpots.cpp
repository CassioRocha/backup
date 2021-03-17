#include <iostream>
#include "touristSpots.h"
using namespace std;
#include <cmath> // sqrt, pow
#include "graph.h"
#include <vector>

void Route::add_node(Node *a, double** matrix)
{
	if(!tour.empty() ){// though the tour first adds the depot, it doesnt count time 
		now_time += matrix[now_idx][a->idx];   // distância de i a j  atual
	}
tour.push_back(*a); 
now_score += a->score; // score_acumulado
TI_acumulado  += a->Ti;		
now_time += matrix[now_idx][a->idx];   // distância de i a j  atual
   open_now   = a->tw_open;
now_idx = a->idx;
a->visited = true;
}




bool Route::timewindows(Node a, double** matrix) const
{
double time_point_to_point = now_time+  matrix[now_idx][a.idx]; // tempo de viagem do ponto i ao ponto j
double time_point_to_hostel = matrix[a.idx][0]; // tempo de viagem do ponto j ao hotel 
double tempodepermanencia=  a.Ti + TI_acumulado ; // tempo de permanencia no ponto turístico 
double ok_time = time_point_to_point  + time_point_to_hostel +tempodepermanencia;
		// adiciona o nó se atendar a janela 
	if (  a.tw_open <= now_time && time_point_to_point + a.Ti  <= a.tw_close   && a.tw_open+ tempodepermanencia  + time_point_to_hostel  <= tmax){ 
	//if (  a.tw_open <= ok_time <= a.tw_close && ok_time + time_point_to_hostel   <= tmax){ 
 cout  << "tempodepermanencia" << tempodepermanencia<< endl;
 cout  << "ok_time" << ok_time<< endl;

        cout << "now_time "<<now_time <<" (int)matrix[now_idx][a.idx] " <<(int)matrix[now_idx][a.idx]<< "     i" << now_idx<<"        j"<< a.idx << endl;

	    	return true;
	}else
		{
			return false;
		}

}


/*
bool Route::timeorder(Node a, double** matrix) const
{
	
	int travel_time = (int)matrix[now_idx][a.idx] ;
	int arrival_time = now_time + travel_time;
	if(a.tw_open <= arrival_time && arrival_time  <= a.tw_close){
	
		return true;// arrive after opening && leave before closing
	}
	else{
		return false;
	}

}
	
*/	