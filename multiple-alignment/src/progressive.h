#ifndef _PROGRESSIVE_H_
#define _PROGRESSIVE_H_

#include <utility>
#include "msa.h"

class TProgressive: public TMsa{
private:
	std::vector<std::vector<float> > m_distances;
	std::vector<std::vector<float> > m_q;
	int idx_center;
public:
	TProgressive(int _match, int _mismatch, int _gap)
		: TMsa(_match, _mismatch, _gap){
	}

	~TProgressive(){	};

	void  FDistances();
	float FPairwiseDistance(std::string, std::string);
	void  FPrintDistances();
	std::pair<int, int> FQ();
	void FPrintQ();

};

float TProgressive::FPairwiseDistance(std::string _a, std::string _b){
	float ngap = 0;
	float ndif = 0;
	float ntot = (float)_a.size();

	for(unsigned i=0; i<_a.size(); i++){
		if(_a[i]=='-' or _b[i]=='-')
			ngap += 1;
		else if(_a[i] != _b[i])
			ndif += 1;
	}

	return (float)ndif/(ntot-ngap);
}

void TProgressive::FDistances(){
	this->m_distances = std::vector<std::vector<float> >(m_dnas.size(),
		std::vector<float>(m_dnas.size()));

	this->m_q = std::vector<std::vector<float> >(m_dnas.size(),
		std::vector<float>(m_dnas.size()));

	TPairwiseAlignment tmp;
	unsigned i,j;
	float tdis;

	for(i=0; i<m_matrix_alignments.size(); i++){
		m_distances[i][i] = 0;
		for(j=i+1; j<m_matrix_alignments[i].size(); j++){
			tmp = m_matrix_alignments[i][j];
			tdis = FPairwiseDistance(tmp.m_alignment.first,
									tmp.m_alignment.second);

			this->m_distances[i][j] = tdis;
			this->m_distances[j][i] = tdis;
		}			
	}
}

void TProgressive::FPrintDistances(){
	unsigned i,j;
	for(i=0; i<m_distances.size(); i++){
		for(j=0; j<m_distances[i].size(); j++){
			printf("%.3lf ", m_distances[i][j]);
		}
		std::cout << "\n";
	}
}

void TProgressive::FPrintQ(){
	unsigned i,j;
	for(i=0; i<m_q.size(); i++){
		for(j=0; j<m_q[i].size(); j++){
			printf("%.3lf ", m_q[i][j]);
		}
		std::cout << "\n";
	}
}

std::pair<int, int> TProgressive::FQ(){
	std::vector<float> v_temp(m_distances.size());

	unsigned i, j;

	float init;
	for(i=0; i<m_distances.size(); i++){
		init = 0;
		for(j=0; j<m_distances[i].size(); j++){
			init += m_distances[i][j];
		}
		v_temp[i] = init;
	}

	float qtmp;
	float n = (float)m_distances.size();

	float dmin = 999999;
	std::pair<int, int> pmin;

	for(i=0; i<m_q.size(); i++){
		for(j=i+1; j<m_q[i].size(); j++){
			qtmp = m_distances[i][j]-((v_temp[i] + v_temp[j])/(n-2));
			m_q[i][j] = qtmp;
			m_q[j][i] = qtmp;

			if(m_q[i][j] < dmin){
				dmin = m_q[i][j];
				pmin.first  = i;
				pmin.second = j;
			}
		}
	}

	return pmin;
}

#endif