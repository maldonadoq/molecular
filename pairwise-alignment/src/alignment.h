#ifndef _ALIGNMENT_H_
#define _ALIGNMENT_H_

#include <string.h>
#include <iostream>
#include <vector>
#include "../../utils/print.h"

#define TMAX 1000
#define PATH 3
int DIRECTION[3][2] = {{0,-1},{-1,-1},{-1,0}};

typedef std::vector<std::pair<std::string, std::string> > Avector;

class TAlignment{
protected:
	std::string m_dna[2];				// dna string
	unsigned n, m;						// matrix
	int match_score;					// match score
	int mismatch_score;					// mismatch score
	int gap_score;						// gap score
public:	
	int  **m_dp;				// weight matrix
	char ***m_back;				// matrix to do the traceback

	TAlignment(std::string, std::string, int, int, int);
	TAlignment(int, int, int);
	TAlignment(){	};
	virtual ~TAlignment();
	
	int  FMakeMatrix();
	void FPrintBackMatrix();
	void FPrintWeightMatrix();
	int  FSimilarity(char, char);
	virtual void FInit(std::string, std::string);

	Avector FGetBestAlignment(Avector, float, float);
	float FGetPenalty(std::pair<std::string, std::string>, float, float);
};

TAlignment::TAlignment(int _match, int _mismatch, int _gap){	
	this->match_score    = _match;
	this->mismatch_score = _mismatch;
	this->gap_score      = _gap;
}

void TAlignment::FInit(std::string _dnaa, std::string _dnab){
	if((_dnaa.size() <  TMAX) and (_dnab.size() < TMAX)){
		this->m_dna[0] = _dnaa;
		this->m_dna[1] = _dnab;

		this->n = m_dna[0].size()+1;
		this->m = m_dna[1].size()+1;
	
		this->m_dp   = new int*[this->n];
		this->m_back = new char**[this->n];
		for(unsigned i=0; i<this->n; i++){
			this->m_dp[i] = new int[this->m];
			this->m_back[i] = new char*[this->m];
			for(unsigned j=0; j<this->m; j++){
				this->m_back[i][j] = new char[PATH];
				for(unsigned k=0; k<PATH; k++){
					this->m_back[i][j][k] = '0';
				}
			}
		}
	}
}

TAlignment::TAlignment(std::string _dnaa, std::string _dnab,
	int _match, int _mismatch, int _gap){	
	

	this->match_score    = _match;
	this->mismatch_score = _mismatch;
	this->gap_score      = _gap;
	
	FInit(_dnaa, _dnab);
}

int TAlignment::FSimilarity(char _a, char _b){
	return (_a == _b)? match_score: -mismatch_score;
}

void TAlignment::FPrintWeightMatrix(){
	print_matrix(m_dp, n, m);
}

void TAlignment::FPrintBackMatrix(){
	print_matrix(m_back, n, m, PATH);
}

float TAlignment::FGetPenalty(std::pair<std::string, std::string> _align, float _p, float _q){
	float penal = 0;

	int k = 0;
	for(unsigned i=0; i<_align.first.size(); i++){
		if(_align.first[i] == '-')	k++;
		else{
			if(k!=0){
				penal += (_p + ((k-1)*_q));
			}
			k = 0;
		}
	}
	for(unsigned i=0; i<_align.second.size(); i++){
		if(_align.second[i] == '-')	k++;
		else{
			if(k!=0){
				penal += (_p + ((k-1)*_q));
			}
			k = 0;
		}
	}

	return penal;
}

Avector TAlignment::FGetBestAlignment(Avector _al, float _p, float _q){
	Avector vtmp;	
	if(_q < _p){
		std::vector<float> vpenalty;
		std::pair<std::string, std::string> talign = _al[0];
		float pmin = FGetPenalty(_al[0],_p,_q);
		float ptmp;
		vpenalty.push_back(pmin);

		for(unsigned i=1; i<_al.size(); i++){
			ptmp = FGetPenalty(_al[i],_p,_q);
			vpenalty.push_back(ptmp);
			if(ptmp < pmin){
				pmin = ptmp;
			}
		}

		// std::cout << "penalty: " << pmin << "\n";

		for(unsigned i=0; i<vpenalty.size(); i++){
			if(vpenalty[i] == pmin)
				vtmp.push_back(_al[i]);
		}
	}

	return vtmp;
}

TAlignment::~TAlignment(){
	delete []this->m_dp;
	delete []this->m_back;
}

#endif