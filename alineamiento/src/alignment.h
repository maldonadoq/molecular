#ifndef _ALIGNMENT_H_
#define _ALIGNMENT_H_

#include <string.h>
#include <iostream>
#include "utils.h"

#define TMAX 1000
#define PATH 3


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
	virtual ~TAlignment();
	
	int  FMakeMatrix();
	void FPrintBackMatrix();
	void FPrintWeightMatrix();
	int  FSimilarity(char, char);
};

TAlignment::TAlignment(std::string _dnaa, std::string _dnab,
	int _match, int _mismatch, int _gap){	

	if((_dnaa.size() <  TMAX) and (_dnab.size() < TMAX)){
		this->m_dna[0] = _dnaa;
		this->m_dna[1] = _dnab;

		this->n = m_dna[0].size()+1;
		this->m = m_dna[1].size()+1;

		this->match_score    = _match;
		this->mismatch_score = _mismatch;
		this->gap_score      = _gap;
	
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

int TAlignment::FSimilarity(char _a, char _b){
	return (_a == _b)? match_score: -mismatch_score;
}

void TAlignment::FPrintWeightMatrix(){
	print_matrix(m_dp, n, m);
}

void TAlignment::FPrintBackMatrix(){
	print_matrix(m_back, n, m, PATH);
}

TAlignment::~TAlignment(){
	delete []this->m_dp;
}

#endif