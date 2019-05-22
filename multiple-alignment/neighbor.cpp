#include <iostream>
#include <vector>
#include <math.h>

void PrintV(std::vector<float> & _v){
	unsigned i;
	for(i=0; i<_v.size(); i++)
		printf("%.3lf\t", _v[i]);
}

void PrintM(std::vector<std::vector<float> > & _v){
	unsigned i,j;
	for(i=0; i<_v.size(); i++){
		for(j=0; j<_v[i].size(); j++){
			printf("%.3lf\t", _v[i][j]);
		}
		std::cout << "\n";
	}
}

void RemoveItem(std::vector<std::vector<float> > & _mat, int _erase){
	_mat.erase(_mat.begin() + _erase);

	for(unsigned i=0; i<_mat.size(); i++){
		_mat[i].erase(_mat[i].begin() + _erase);
	}
}

void Neighbor(std::vector<std::vector<float> > _distances){

	std::vector<std::vector<float> > D = _distances, Q, DT;

	std::vector<float> vsum;

	unsigned tsize, i, j;
	float init, qtmp, dmin = 999999, dtmp;

	int max_id, min_id;

	std::pair<int, int> pmin;

	while(D.size() > 2){
		DT = D;
		std::cout << "\nD:\n";
		PrintM(D);

		tsize = D.size();
		vsum.resize(tsize);

		for(i=0; i<tsize; i++){
			init = 0;
			for(j=0; j<D[i].size(); j++){
				init += D[i][j];
			}
			vsum[i] = init;
		}
		
		Q = std::vector<std::vector<float> >(tsize,std::vector<float>(tsize));

		for(i=0; i<Q.size(); i++){
			for(j=i+1; j<Q[i].size(); j++){
				qtmp = vsum[i] + vsum[j];
				Q[i][j] = D[i][j]-(qtmp/((float)tsize-2));
				Q[j][i] = D[i][j]-(qtmp/((float)tsize-2));

				if(Q[i][j] < dmin){
					dmin = Q[i][j];
					pmin.first  = i;
					pmin.second = j;
				}			
			}
		}
		
		max_id = std::max(pmin.first, pmin.second);
		min_id = std::min(pmin.first, pmin.second);

		std::cout << "\nQ:\n";
		PrintM(Q);
		std::cout << "\nQ Minimun: [" << pmin.first << "," << pmin.second << "]\n\n";

		RemoveItem(D, max_id);
		
		// incomplete
		j = min_id;
		for(i=0; i<DT.size(); i++){
			if(((int)i != min_id) and ((int)i != max_id)){
				dtmp = (DT[min_id][i] + DT[max_id][i] - DT[min_id][max_id])/2;
				D[min_id][j] = dtmp;
				D[j][min_id] = dtmp;
				j++;
				std::cout << dtmp << " ";
			}
		}
		std::cout << "\n";
		D[min_id][min_id] = 0;

	}	
}

int main(int argc, char const *argv[]){
	unsigned n = 6;

	std::vector<std::vector<float> > distances;
	distances = std::vector<std::vector<float> >(n);

	distances[0] = {0,5,4,7,6,8};
	distances[1] = {5,0,7,10,9,11};
	distances[2] = {4,7,0,7,6,8};
	distances[3] = {7,10,7,0,5,9};
	distances[4] = {6,9,6,5,0,8};
	distances[5] = {8,11,8,9,8,0};

	Neighbor(distances);
	return 0;
}