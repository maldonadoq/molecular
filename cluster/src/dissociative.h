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
	std::map<H, std::map<H, T> > m_distances;	
	std::map<H, T> m_individual_distance;
	std::vector<std::string> m_cluster;
public:
	TDissociative();
	~TDissociative();

	void Init(std::vector<std::vector<T > >, std::vector<H>);
	void Cluster();

	std::pair<H,T> FindMax();
	T TypeC(std::vector<H>, H);
};

template <class T, class H>
TDissociative<T,H>::TDissociative(){

}

template <class T, class H>
void TDissociative<T,H>::Init(std::vector<std::vector<T > > _distances, std::vector<H> _headers){
	if(_distances.size() != _headers.size()){
		std::cout << "Error: matrix should be [" << _headers.size() << "x" << _headers.size() << "]\n";
		return;
	}

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
	T tmin = m_distances[_value][_vect[0]];
	T ttmp;

	for(unsigned i=1; i<_vect.size(); i++){
		ttmp = m_distances[_value][_vect[i]];
		if(ttmp < tmin){
			tmin = ttmp;
		}
	}

	return tmin;
}

template <class T, class H>
void TDissociative<T,H>::Cluster(){	
	std::pair<H,T> tmax;

	typename std::map<H,T>::iterator it;
	T tpr, tnew, tdif, tvalue;

	std::vector<H> rvector;		// remove vector

	unsigned i;
	H snew;	
	T tmin;
	while(m_individual_distance.size() > 2){
		tmax = FindMax();
		m_individual_distance.erase(tmax.first);
		snew = tmax.first;

		rvector.clear();

		for(it=m_individual_distance.begin(); it!=m_individual_distance.end(); it++){
			tpr = it->second;
			tnew = m_distances[tmax.first][it->first];
			tdif = tpr - tnew;

			std::cout << std::setw(4) << std::fixed << std::left << std::setprecision(2) \
				<< it->first << "\t" << tpr << "\t" << tnew << "\t" << tdif << "\n";

			if(tdif >= 0){				
				rvector.push_back(it->first);
				snew += it->first;
			}
		}

		m_cluster.push_back(snew);
		std::cout << "CL: " << snew << "\n\n";

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
	}
}

template <class T, class H>
TDissociative<T,H>::~TDissociative(){
	this->m_distances.clear();
}

#endif