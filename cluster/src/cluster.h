#ifndef _CLUSTER_H_
#define _CLUSTER_H_

#include <iostream>
#include <utility>
#include <vector>
#include <map>

template <class T, class H, class C>
class TCluster{
private:
	C m_op;
	std::map<H, std::map<H, T> > m_distances;
	std::vector<H> m_headers;

public:
	TCluster();
	~TCluster();

	void SetHeader(std::vector<H>);
	void SetDistance(std::vector<std::vector<T > >);
	void PrintMap();
	void Cluster();
	std::pair<int, int> FindMin();
};

template <class T, class H, class C>
TCluster<T,H,C>::TCluster(){

}

template <class T, class H, class C>
void TCluster<T,H,C>::SetHeader(std::vector<H> _headers){
	this->m_headers = _headers;
}

template <class T, class H, class C>
void TCluster<T,H,C>::SetDistance(std::vector<std::vector<T > > _distances){
	if(_distances.size() != m_headers.size()){
		std::cout << "Error: matrix should be [" << m_headers.size() << "x" << m_headers.size() << "]\n";
		return;
	}

	unsigned i,j;
	for(i=0; i<_distances.size(); i++){
		for(j=0; j<_distances[i].size(); j++){
			m_distances[m_headers[i]][m_headers[j]] = _distances[i][j];
			m_distances[m_headers[j]][m_headers[i]] = _distances[i][j];
		}
	}
}

template <class T, class H, class C>
std::pair<int, int> TCluster<T,H,C>::FindMin(){
	std::pair<int, int> pmin = std::make_pair(1,0);
	float tmin = m_distances[m_headers[1]][m_headers[0]];

	int i,j;
	for(i=2; i<(int)m_headers.size(); i++){
		for(j=0; j<=i-1; j++){
			if(m_distances[m_headers[i]][m_headers[j]] < tmin){
				tmin = m_distances[m_headers[i]][m_headers[j]];
				pmin.first  = i;
				pmin.second = j;
			}
		}
	}	

	return pmin;
}

template <class T, class H, class C>
void TCluster<T,H,C>::Cluster(){

	std::map<H, std::map<H, T> > _distances;
	std::vector<H> _headers;
	std::pair<int, int> pmin;
	int t1, t2;
	std::string cl;
	T value;

	T hi, hj;
	while(m_headers.size() > 1){
		PrintMap();
		std::cout << "\n";

		pmin = FindMin();
		t1 = std::min(pmin.first, pmin.second);
		t2 = std::max(pmin.first, pmin.second);

		_headers = m_headers;

		cl = _headers[t1]+_headers[t2];
		m_headers[t1] = cl;
		m_headers.erase(m_headers.begin() + t2);

		for(unsigned i=0; i<m_headers.size(); i++){
			hi = m_distances[_headers[pmin.first ]][m_headers[i]];
			hj = m_distances[_headers[pmin.second]][m_headers[i]];
			value = m_op(hi, hj);

			_distances[cl][m_headers[i]] = value;
			_distances[m_headers[i]][cl] = value;

			for(unsigned j=0; j<m_headers.size(); j++){
				if(!_distances[m_headers[i]][m_headers[j]]){

					_distances[m_headers[i]][m_headers[j]] = m_distances[m_headers[i]][m_headers[j]];
					_distances[m_headers[j]][m_headers[i]] = m_distances[m_headers[i]][m_headers[j]];
				}
			}
		}
		this->m_distances = _distances;
		_distances.clear();
	}
}

template <class T, class H, class C>
void TCluster<T,H,C>::PrintMap(){
	unsigned i,j;

	/*for(i=0; i<m_headers.size(); i++){
		std::cout << m_headers[i] << "\t";
	}
	std::cout << "\n";*/

	for(i=0; i<m_headers.size(); i++){
		for(j=0; j</*=i-1*/m_headers.size(); j++){
			printf("%.2f ", m_distances[m_headers[i]][m_headers[j]]);
		}
		std::cout << "\n";
	}
}

template <class T, class H, class C>
TCluster<T,H,C>::~TCluster(){
	this->m_distances.clear();
}

#endif