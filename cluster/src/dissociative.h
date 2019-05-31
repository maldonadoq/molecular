#ifndef _DISSOCIATIVE_H_
#define _DISSOCIATIVE_H_

#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <iomanip>

using std::unordered_map;
using std::string;
using std::vector;
using std::cout;
using std::pair;

class TDissociative{
private:
	int m_type;
	unordered_map<string, unordered_map<string, float> > m_distances;	
	unordered_map<string, float> m_individual_distance;
	vector<string> m_cluster;
public:
	TDissociative();
	~TDissociative();

	void Init(vector<vector<float > >, vector<string>);
	void SetType(int);
	vector<string> GetClusters();
	void Run(int);

	pair<string,float> FindMax();
	float TypeC(vector<string>, string);
};

TDissociative::TDissociative(){

}

void TDissociative::SetType(int _type){
	this->m_type = _type;
}

void TDissociative::Init(vector<vector<float > > _distances, vector<string> _headers){
	if(_distances.size() != _headers.size()){
		cout << "Error: matrix should be [" << _headers.size() << "x" << _headers.size() << "]\n";
		return;
	}

	m_distances.clear();
	m_individual_distance.clear();
	m_cluster.clear();
	unsigned i,j;	
	for(i=0; i<_distances.size(); i++){
		for(j=0; j<_distances[i].size(); j++){
			m_distances[_headers[i]][_headers[j]] = _distances[i][j];
			m_distances[_headers[j]][_headers[i]] = _distances[i][j];
		}		
	}

	float tmin, tmp;
	for(i=0; i<_distances.size(); i++){
		tmin = 9999999;
		for(j=0; j<_distances.size(); j++){
			tmp = m_distances[_headers[i]][_headers[j]];
			if((tmp < tmin) and (tmp != 0)){
				tmin = tmp;
			}
		}
		m_individual_distance[_headers[i]] = tmin;
	}
}

pair<string,float> TDissociative::FindMax(){
	typename unordered_map<string,float>::iterator it = m_individual_distance.begin();
	pair<string,float> tmax = *it;

	for(it++; it!=m_individual_distance.end(); it++){
    	if(it->second > tmax.second){
    		tmax = *it;
    	}
	}

	return tmax;
}

float TDissociative::TypeC(vector<string> _vect, string _value){
	float tval = m_distances[_value][_vect[0]];
	float tsum = 0;
	float ttmp;

	for(unsigned i=1; i<_vect.size(); i++){
		ttmp = m_distances[_value][_vect[i]];

		switch(m_type){
			case 0:{
				if(ttmp < tval){
					tval = ttmp;
				}
				break;
			}
			case 1:{
				if(ttmp > tval){
					tval = ttmp;
				}
				break;
			}
			case 2:{
				tsum += ttmp;
				break;
			}
		}		
	}

	if(m_type == 2)
		tval = tval/int(_vect.size());

	return tval;
}

void TDissociative::Run(int _n){
	pair<string,float> tmax;

	typename unordered_map<string,float>::iterator it;
	float tpr, tnew, tdif, tvalue;

	vector<string> rvector;

	unsigned i;
	string snew;	
	float tmin;

	while((int)m_cluster.size() < (_n-1) and (m_individual_distance.size() > 2)){
		tmax = FindMax();
		m_individual_distance.erase(tmax.first);
		snew = tmax.first;

		rvector.clear();
		for(it=m_individual_distance.begin(); it!=m_individual_distance.end(); it++){
			tpr = it->second;
			tnew = m_distances[tmax.first][it->first];
			tdif = tpr - tnew;

			if(tdif >= 0){				
				rvector.push_back(it->first);
				snew += "-"+it->first;
			}
		}

		if(rvector.size() > 0){
			for(i=0; i<rvector.size(); i++){
				m_individual_distance.erase(rvector[i]);
			}

			tmin = 99999;
			for(it=m_individual_distance.begin(); it!=m_individual_distance.end(); it++){
				tvalue = TypeC(rvector, it->first);
				m_distances[snew][it->first] = tvalue;
				m_distances[it->first][snew] = tvalue;

				if(tvalue < tmin){
					tmin = tvalue;
				}
			}

			m_individual_distance[snew] = tmin;	

			for(i=0; i<rvector.size(); i++){
				m_distances.erase(rvector[i]);
			}
		}
		else{
			m_cluster.push_back(tmax.first);
		}

		m_distances.erase(tmax.first);
	}

	snew = "";
	for(it=m_individual_distance.begin(); it!=m_individual_distance.end(); it++){
		snew += "-"+it->first;
	}

	m_cluster.push_back(snew.substr(1));
}

vector<string> TDissociative::GetClusters(){
	return m_cluster;
}

TDissociative::~TDissociative(){
	this->m_distances.clear();
}

#endif