#ifndef _AGGLOMERATIVE_H_
#define _AGGLOMERATIVE_H_

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <iomanip>
#include "utils.h"

using std::make_pair;
using std::string;
using std::vector;
using std::cout;
using std::pair;

class TUpgma{
private:
	vector<vector<float> > m_distances;
	vector<string> m_headers;

public:
	TUpgma();
	~TUpgma();

	void Init(vector<vector<float > >, vector<string>);
	void Run(unsigned);
	pair<int, int> FindMin();
	vector<string> GetClusters();
};

TUpgma::TUpgma(){

}

void TUpgma::Init(vector<vector<float > > _distances, vector<string> _headers){
	if(_distances.size() != _headers.size()){
		cout << "Error: matrix should be [" << _headers.size() << "x" << _headers.size() << "]\n";
		return;
	}
	m_headers = _headers;

	m_distances.clear();
	m_distances = std::vector<vector<float> >(_headers.size(), vector<float>(_headers.size()));

	unsigned i,j;
	for(i=0; i<_distances.size(); i++){
		for(j=0; j<_distances[i].size(); j++){
			m_distances[i][j] = _distances[i][j];
			m_distances[j][i] = _distances[i][j];
		}
	}
}

pair<int, int> TUpgma::FindMin(){
	pair<int, int> pmin = make_pair(1,0);
	float tmin = m_distances[1][0];

	int i,j;
	for(i=2; i<(int)m_headers.size(); i++){
		for(j=0; j<i; j++){
			if(m_distances[i][j] < tmin){
				tmin = m_distances[i][j];
				pmin.first  = i;
				pmin.second = j;
			}
		}
	}

	return pmin;
}

void TUpgma::Run(unsigned _n){
	pair<int, int> pmin;
	string cl, cl1, cl2;
	float value;

	float hi, hj, ha;
	int tmp;

	while(m_headers.size() > _n){
		// print_vector(m_headers);
		pmin = FindMin();

		if(pmin.first > pmin.second){
			tmp = pmin.first;
			pmin.first = pmin.second;
			pmin.second = tmp;
		}

		cl1 = m_headers[pmin.first];
		cl2 = m_headers[pmin.second];
		cl = cl1 + "-" + cl2;
		
		// cout << m_distances[pmin.first][pmin.second] << "\n";

		m_headers[pmin.first] = cl;
		m_headers.erase(m_headers.begin() + pmin.second);

		for(unsigned i=0; i<m_distances.size(); i++){
			hi = m_distances[i][pmin.first];
			hj = m_distances[i][pmin.second];
			ha = (hi+hj)/2;

			m_distances[i][pmin.first] = ha;
			m_distances[pmin.first][i] = ha;
		}

		m_distances.erase(m_distances.begin() + pmin.second);
		for(unsigned i=0; i<m_distances.size(); i++){
			m_distances[i].erase(m_distances[i].begin() + pmin.second);
		}

		m_distances[pmin.first][pmin.first] = 0;
	}
	print_vector(m_headers);
}

vector<string> TUpgma::GetClusters(){
	return m_headers;
}

TUpgma::~TUpgma(){
	this->m_distances.clear();
	this->m_headers.clear();
}

#endif