#ifndef _AGGLOMERATIVE_H_
#define _AGGLOMERATIVE_H_

#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <iomanip>

template <class T, class H, class C>
class TAgglomerative{
private:
	C m_op;
	std::map<H, std::map<H, T> > m_distances;
	std::vector<H> m_headers;

public:
	TAgglomerative();
	~TAgglomerative();

	void Init(std::vector<std::vector<T > >, std::vector<H>);
	void Run(unsigned);
	std::pair<int, int> FindMin();
	std::vector<H> GetClusters();
};

template <class T, class H, class C>
TAgglomerative<T,H,C>::TAgglomerative(){

}


template <class T, class H, class C>
void TAgglomerative<T,H,C>::Init(std::vector<std::vector<T > > _distances, std::vector<H> _headers){
	if(_distances.size() != _headers.size()){
		std::cout << "Error: matrix should be [" << _headers.size() << "x" << _headers.size() << "]\n";
		return;
	}
	m_headers = _headers;

	m_distances.clear();
	unsigned i,j;
	for(i=0; i<_distances.size(); i++){
		for(j=0; j<_distances[i].size(); j++){
			m_distances[m_headers[i]][m_headers[j]] = _distances[i][j];
			m_distances[m_headers[j]][m_headers[i]] = _distances[i][j];
		}
	}
}

template <class T, class H, class C>
std::pair<int, int> TAgglomerative<T,H,C>::FindMin(){
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
void TAgglomerative<T,H,C>::Run(unsigned _n){
	std::pair<int, int> pmin;
	std::string cl, cl1, cl2;
	T value;

	T hi, hj;
	while(m_headers.size() > _n){
		pmin = FindMin();

		cl1 = m_headers[pmin.first];
		cl2 = m_headers[pmin.second];
		cl = cl1 + "-" + cl2;
		
		m_headers[std::min(pmin.first, pmin.second)] = cl;
		m_headers.erase(m_headers.begin() + std::max(pmin.first, pmin.second));

		for(unsigned i=0; i<m_headers.size(); i++){
			hi = m_distances[cl1][m_headers[i]];
			hj = m_distances[cl2][m_headers[i]];
			value = m_op(hi, hj);

			m_distances[cl][m_headers[i]] = value;
			m_distances[m_headers[i]][cl] = value;
		}

		m_distances.erase(cl1);
		m_distances.erase(cl2);
	}
}

template <class T, class H, class C>
std::vector<H> TAgglomerative<T,H,C>::GetClusters(){
	return m_headers;
}

template <class T, class H, class C>
TAgglomerative<T,H,C>::~TAgglomerative(){
	this->m_distances.clear();
	this->m_headers.clear();
}

#endif