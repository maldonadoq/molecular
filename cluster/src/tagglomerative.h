#ifndef _AGGLOMERATIVE_string_
#define _AGGLOMERATIVE_string_

#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>

using namespace std::chrono;

using std::unordered_map;
using std::make_pair;
using std::string;
using std::thread;
using std::vector;
using std::cout;
using std::pair;
using std::max;
using std::min;

struct TInfo
{
	int i;
	int j;
	float value;
};

void PrintPair(pair<int, int> _p, string _name){
	cout << _name << "[" << _p.first << "," << _p.second << "]\n";
}

template <class C>
class TAgglomerative{
private:	
	C m_op;
	static vector<TInfo> m_minumun;
	static unordered_map<string, unordered_map<string, float> > m_distances;
	static vector<string> m_headers;

	static void MinParallel(int);
public:
	TAgglomerative();
	~TAgglomerative();

	void Init(vector<vector<float > >, vector<string>);
	void Run(unsigned);
	pair<int, int> FindMin();
	pair<int, int> FindMinNormal();
	pair<int, int> FindMinParallel();
	vector<string> GetClusters();
};

template <class C>	unordered_map<string, unordered_map<string, float> > TAgglomerative<C>::m_distances;
template <class C>	vector<TInfo> TAgglomerative<C>::m_minumun;
template <class C>	vector<string> TAgglomerative<C>::m_headers;

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
pair<int, int> TAgglomerative<C>::FindMinNormal(){
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
void TAgglomerative<C>::MinParallel(int _idx){
	int i;
	TInfo pmin = {_idx, 0, m_distances[m_headers[_idx]][m_headers[0]]};
	TInfo tmp;
	
	for(i=1; i<_idx; i++){
		tmp = {_idx, i, m_distances[m_headers[_idx]][m_headers[i]]};

		if(tmp.value < pmin.value){
			pmin = tmp;
		}
	}

	m_minumun[_idx-2] = pmin;
}

template <class C>
pair<int, int> TAgglomerative<C>::FindMinParallel(){
	TInfo pmin = {1,0,m_distances[m_headers[1]][m_headers[0]]};
	int nv = m_headers.size()-2;
	m_minumun.resize(nv);

	thread thf[nv];

	int i;
	for(i=0; i<nv; i++){
		thf[i] = thread(MinParallel, (int)i+2);
	}

	for(i=0; i<nv; i++){
		thf[i].join();
	}

	for(i=0; i<(int)m_minumun.size(); i++){
		if(m_minumun[i].value < pmin.value){
			pmin = m_minumun[i];
		}
	}

	return make_pair(pmin.i, pmin.j);
}

template <class C>
pair<int, int> TAgglomerative<C>::FindMin(){
	if(m_headers.size() < 400){
		return FindMinNormal();
	}

	return FindMinParallel();
}
template <class C>
void TAgglomerative<C>::Run(unsigned _n){
	pair<int, int> pmin, other;
	string cl, cl1, cl2;
	float value;

	float hi, hj;

	high_resolution_clock::time_point tinit;
	high_resolution_clock::time_point tend;
	duration<double> time_span;

	float tfind = 0;
	float texec = 0;

	while(m_headers.size() > _n){
		tinit = high_resolution_clock::now();
			pmin = FindMin();
			// pmin = FindMinNormal();
		tend = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(tend - tinit);
		tfind += time_span.count();

		tinit = high_resolution_clock::now();

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

		tend = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(tend - tinit);
		texec += time_span.count();
	}

	cout << "[time find] : " << tfind << " s\n";
	cout << "[time exec] : " << texec << " s\n";
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