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

#endif