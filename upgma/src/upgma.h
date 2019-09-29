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
	vector<TItem> m_headers;

public:
	TUpgma();
	~TUpgma();

	void Init(vector<vector<float > >, vector<string>);
	void Run(unsigned);
	pair<int, int> FindMin();
	vector<TItem> GetClusters();
};

TUpgma::TUpgma(){

}

void TUpgma::Init(vector<vector<float > > _distances, vector<string> _headers){
	if(_distances.size() != _headers.size()){
		cout << "Error: matrix should be [" << _headers.size() << "x" << _headers.size() << "]\n";
		return;
	}

	m_distances.clear();
	m_headers.clear();

	unsigned i,j;
	for(i=0; i<_headers.size(); i++){
		m_headers.push_back({_headers[i], 1, i, {0,0}, 0});
	}

	m_distances = std::vector<vector<float> >(_headers.size(), vector<float>(_headers.size()));	
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
	TItem cl, cl1, cl2;

	float hi, hj, ha;
	int tmp;

	unsigned tidx = m_headers.size();
	string _body = "upgma_labels = [";
	for(unsigned i=0; i<tidx-1; i++){
		_body += "\"" + m_headers[i].m_value + "\",";
	}

	_body += "\"" + m_headers[tidx-1].m_value + "\"]\n";
	_body += "upgma_values = [\n";

	while(m_headers.size() > _n){
		pmin = FindMin();

		if(pmin.first > pmin.second){
			tmp = pmin.first;
			pmin.first = pmin.second;
			pmin.second = tmp;
		}

		// print_matrix(m_distances);
		// cout << "\n";

		cl1 = m_headers[pmin.first];
		cl2 = m_headers[pmin.second];

		cl = {cl1.m_value+"-"+cl2.m_value, cl1.m_number+cl2.m_number, tidx, {cl1.m_idx, cl2.m_idx}, (float)m_distances[pmin.first][pmin.second]/2};
		_body += "\t\t" + item_to_str(cl) + ",\n";
		// cout << item_to_str(cl) << "\n";

		m_headers[pmin.first] = cl;
		m_headers.erase(m_headers.begin() + pmin.second);

		for(unsigned i=0; i<m_distances.size(); i++){
			hi = m_distances[i][pmin.first];
			hj = m_distances[i][pmin.second];
			ha = (float)(hi+hj)/2;

			m_distances[i][pmin.first] = ha;
			m_distances[pmin.first][i] = ha;
		}

		m_distances.erase(m_distances.begin() + pmin.second);
		for(unsigned i=0; i<m_distances.size(); i++){
			m_distances[i].erase(m_distances[i].begin() + pmin.second);
		}

		m_distances[pmin.first][pmin.first] = 0;

		tidx++;
	}

	_body += "\t]\n";
	Create_Python(_body);
}

vector<TItem> TUpgma::GetClusters(){
	return m_headers;
}

TUpgma::~TUpgma(){
	this->m_distances.clear();
	this->m_headers.clear();
}

#endif