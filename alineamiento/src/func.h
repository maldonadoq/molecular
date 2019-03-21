#ifndef _FUNC_H_
#define _FUNC_H_

template<class T>
void print_matrix(T **matrix, unsigned _n, unsigned _m){
	for(unsigned i=0; i<_n; i++){		
		for(unsigned j=0; j<_m; j++){
			std::cout << matrix[i][j] << "\t";
		}
		std::cout << "\n";
	}
}

template<class T>
void print_matrix(T ***matrix, unsigned _n, unsigned _m, unsigned _l){
	for(unsigned i=0; i<_n; i++){		
		for(unsigned j=0; j<_m; j++){
			std::cout << "[";
			for(unsigned k=0; k<_l; k++){
				std::cout << matrix[i][j][k];
				if(k!=_l-1)	std::cout << " ";
			}
			std::cout << "] \t";
		}
		std::cout << "\n";
	}
}

#endif