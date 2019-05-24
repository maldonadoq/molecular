#ifndef _DISSOCIATIVE_H_
#define _DISSOCIATIVE_H_

#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <iomanip>

template <class T, class H>
class TDissociative{
private:
	int m_type;
	std::map<H, std::map<H, T> > m_distances;	
	std::map<H, T> m_individual_distance;
	std::vector<H> m_cluster;
public:
	TDissociative();
	~TDissociative();

	void Init(std::vector<std::vector<T > >, std::vector<H>);
	void SetType(int);
	std::vector<H> GetClusters();
	void Run(int);

	std::pair<H,T> FindMax();
	T TypeC(std::vector<H>, H);
};

template <class T, class H>
TDissociative<T,H>::TDissociative(){

}

template <class T, class H>
void TDissociative<T,H>::SetType(int _type){
	this->m_type = _type;
}

template <class T, class H>
void TDissociative<T,H>::Init(std::vector<std::vector<T > > _distances, std::vector<H> _headers){
	if(_distances.size() != _headers.size()){
		std::cout << "Error: matrix should be [" << _headers.size() << "x" << _headers.size() << "]\n";
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

	T tmin, tmp;
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

template <class T, class H>
std::pair<H,T> TDissociative<T,H>::FindMax(){
	typename std::map<H,T>::iterator it = m_individual_distance.begin();
	std::pair<H,T> tmax = *it;

	for(it++; it!=m_individual_distance.end(); it++){
    	if(it->second > tmax.second){
    		tmax = *it;
    	}
	}

	return tmax;
}

template <class T, class H>
T TDissociative<T,H>::TypeC(std::vector<H> _vect, H _value){
	T tval = m_distances[_value][_vect[0]];
	T tsum = 0;
	T ttmp;

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

template <class T, class H>
void TDissociative<T,H>::Run(int _n){
	std::pair<H,T> tmax;

	typename std::map<H,T>::iterator it;
	T tpr, tnew, tdif, tvalue;

	std::vector<H> rvector;		// remove vector

	unsigned i;
	H snew;	
	T tmin;
	// int count = 0;

	while((int)m_cluster.size() < (_n-1) and (m_individual_distance.size() > 2)){

		/*for(it=m_individual_distance.begin(); it!=m_individual_distance.end(); it++){
			std::cout << it->first << " ";
		}
		std::cout << "\n\n";*/

		tmax = FindMax();
		m_individual_distance.erase(tmax.first);
		snew = tmax.first;// +"-";

		rvector.clear();
		for(it=m_individual_distance.begin(); it!=m_individual_distance.end(); it++){
			tpr = it->second;
			tnew = m_distances[tmax.first][it->first];
			tdif = tpr - tnew;

			/* std::cout << std::setw(4) << std::fixed << std::left << std::setprecision(2) \
				<< it->first << "\t" << tpr << "\t" << tnew << "\t" << tdif << "\n"; */

			if(tdif >= 0){				
				rvector.push_back(it->first);
				snew += it->first;// +"-";
			}
		}

		if(rvector.size() > 0){
			// snew = snew.substr(0,snew.size()-1);
			// tmp.substr(0, tmp.size()-1
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
		snew += it->first;// +"-";
	}
	// std::cout << "\n------------\n";
	// snew = snew.substr(0,snew.size()-1);

	m_cluster.push_back(snew);
}

template <class T, class H>
std::vector<H> TDissociative<T,H>::GetClusters(){
	return m_cluster;
}

template <class T, class H>
TDissociative<T,H>::~TDissociative(){
	this->m_distances.clear();
}

#endif