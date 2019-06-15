#ifndef _PRINT_H_
#define _PRINT_H_

#include <utility>
#include <vector>

using std::vector;
using std::cout;

template<class T>
void print_matrix(T **matrix, unsigned _n, unsigned _m){
	for(unsigned i=0; i<_n; i++){		
		for(unsigned j=0; j<_m; j++){
			std::cout << matrix[i][j] << "  ";
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
			}
			std::cout << "]  ";
		}
		std::cout << "\n";
	}
}

template<class T>
void print_matrix(vector<vector<T> > _matrix){
	unsigned i,j;
	for(i=0; i<_matrix.size(); i++){
		for(j=0; j<_matrix[i].size(); j++){
			cout << _matrix[i][j] << "\t";
		}
		cout << "\n";
	}
}

template <class T>
void print_pair(std::pair<T, T> _pair){
	std::cout << "A1: [" << _pair.first << "]\nA2: [" << _pair.second << "]\n";
}

template <class T>
void print_vector_pair(std::vector<std::pair<T, T> > &vec){
	for(unsigned i=0; i<vec.size(); i++)
		std::cout << "[" << vec[i].first << "," << vec[i].second << "] ";
	std::cout << "\n";
}

template <class T>
void print_vector_pair_a(std::vector<std::pair<T, T> > &vec){
	for(unsigned i=0; i<vec.size(); i++)
		std::cout << vec[i].first << "\n" << vec[i].second << "\n\n";
}

template <class T>
bool is_not_visited(std::vector<std::pair<T, T> > &vec, std::pair<T,T> t){
	for(unsigned i=0; i<vec.size(); i++){
		if(vec[i] == t)
			return false;
	}

	return true;
}

template<class T>
void print_vector(std::vector<T> vec){
	for(unsigned i=0; i<vec.size(); i++)
		std::cout << vec[i] << " ";
	std::cout << "\n";
}

template<class T>
void print_vector_r(std::vector<T> vec, T r){
	for(unsigned i=0; i<vec.size(); i++)
		std::cout << r << "\n" << vec[i] << "\n\n";
}

template<class T>
void print_n(T m, unsigned n){
	for(unsigned i=0; i<n; i++)
		std::cout << m << " ";
	std::cout << "\n";
}

template<class T>
void print_vector_t(std::vector<T> vec){
	for(unsigned i=0; i<vec.size(); i++)
		std::cout << vec[i] << "\n";
}

#endif