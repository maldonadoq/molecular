#ifndef _AGGLOMERATIVE_H_
#define _AGGLOMERATIVE_H_

#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <iomanip>

using std::unordered_map;
using std::make_pair;
using std::string;
using std::vector;
using std::cout;
using std::pair;
using std::max;
using std::min;

template <class C>
class TAgglomerative{
private:
	C m_op;
	unordered_map<string, unordered_map<string, float> > m_distances;
	vector<string> m_headers;

public:
	TAgglomerative();
	~TAgglomerative();

	void Init(vector<vector<float > >, vector<string>);
	void Run(unsigned);
	pair<int, int> FindMin();
	vector<string> GetClusters();
};

template <class C>
TAgglomerative<C>::TAgglomerative(){

}


template <class C>
void TAgglomerative<C>::Init(vector<vector<float > > _distances, vector<string> _headers){
	if(_distances.size() != _headers.size()){
		cout << "Error: matrix should be [" << _headers.size() << "x" << _headers.size() << "]\n";
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

template <class C>
pair<int, int> TAgglomerative<C>::FindMin(){
	pair<int, int> pmin = make_pair(1,0);
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

template <class C>
void TAgglomerative<C>::Run(unsigned _n){
	pair<int, int> pmin;
	string cl, cl1, cl2;
	float value;

	float hi, hj;
	while(m_headers.size() > _n){
		pmin = FindMin();

		cl1 = m_headers[pmin.first];
		cl2 = m_headers[pmin.second];
		cl = cl1 + "-" + cl2;
		
		m_headers[min(pmin.first, pmin.second)] = cl;
		m_headers.erase(m_headers.begin() + max(pmin.first, pmin.second));

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

template <class C>
vector<string> TAgglomerative<C>::GetClusters(){
	return m_headers;
}

template <class C>
TAgglomerative<C>::~TAgglomerative(){
	this->m_distances.clear();
	this->m_headers.clear();
}

#endif