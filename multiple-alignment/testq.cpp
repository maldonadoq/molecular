#include <iostream>
#include <vector>

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

void CalculateQ(std::vector<std::vector<float> > &_distances, std::vector<std::vector<float> > &_q){
	std::vector<float> v_temp(_distances.size());
	unsigned i, j;

	float init;
	for(i=0; i<_distances.size(); i++){
		init = 0;
		for(j=0; j<_distances[i].size(); j++){
			init += _distances[i][j];
		}
		v_temp[i] = init;
	}

	// PrintV(v_temp);

	float qtmp;
	float n = (float)_distances.size();

	float dmin = 999999;
	std::pair<int, int> pmin;

	for(i=0; i<_q.size(); i++){
		for(j=i+1; j<_q[i].size(); j++){
			qtmp = v_temp[i] + v_temp[j];
			_q[i][j] = _distances[i][j]-(qtmp/(n-2));
			_q[j][i] = _distances[i][j]-(qtmp/(n-2));

			if(_q[i][j] < dmin){
				dmin = _q[i][j];
				pmin.first  = i;
				pmin.second = j;
			}			
		}
	}

	std::cout << "\nMinimun: [" << pmin.first << "," << pmin.second << "]\n";
}

int main(int argc, char const *argv[]){
	unsigned n = 6;

	std::vector<std::vector<float> > distances;
	std::vector<std::vector<float> > q;

	q = std::vector<std::vector<float> >(n,std::vector<float>(n));
	distances = std::vector<std::vector<float> >(n);

	distances[0] = {0,5, 4,7, 6,8};
	distances[1] = {5,0, 7,10,9,11};
	distances[2] = {4,7, 0,7, 6,8};
	distances[3] = {7,10,7,0, 5,9};
	distances[4] = {6,9, 6,5, 0,8};	
	distances[5] = {8,11,8,9, 8,0};

	PrintM(distances);
	std::cout << "\n";
	PrintM(q);

	CalculateQ(distances,q);

	std::cout << "\n\n";

	std::cout << "bool: " << sizeof(bool) << "\n";
	std::cout << "char: " << sizeof(char) << "\n";

	PrintM(q);

	return 0;
}