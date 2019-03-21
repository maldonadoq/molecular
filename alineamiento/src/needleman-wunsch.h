#ifndef _NEEDLEMAN_WUNSCH_H_
#define _NEEDLEMAN_WUNSCH_H_

#include <string.h>
#include <iostream>
#include <utility>
#include <algorithm>
#include "func.h"

extern unsigned TMAX;

class TNeedlemanWunsch{
private:
	std::string m_dna[2];		// dna string
	unsigned n, m;				// matrix
	int match_score;			// match score
	int mismatch_score;			// mismatch score
	int gap_score;				// gap score
public:	
	int **m_dp;					// matrix

	TNeedlemanWunsch(std::string, std::string, int, int, int);
	~TNeedlemanWunsch();

	int FMakeMatrix();
	void FPrintMatrix();
	std::pair<std::string, std::string> FGlobalOptimum();
	int FSimilarity(char, char);
};

TNeedlemanWunsch::TNeedlemanWunsch(std::string _dnaa, std::string _dnab,
	int _match, int _mismatch, int _gap){

	this->m_dna[0] = _dnaa;
	this->m_dna[1] = _dnab;

	this->n = this->m_dna[0].size()+1;
	this->m = this->m_dna[1].size()+1;

	this->match_score    = _match;
	this->mismatch_score = _mismatch;
	this->gap_score      = _gap;

	if((this->n <  TMAX) and (this->m < TMAX)){
		this->m_dp = new int*[this->n];
		for(unsigned i=0; i<this->n; i++){
			this->m_dp[i] = new int[this->m];
		}
	}	
}

int TNeedlemanWunsch::FSimilarity(char _a, char _b){
	return (_a == _b)? match_score: -mismatch_score;
}

void TNeedlemanWunsch::FPrintMatrix(){
	print_matrix(m_dp, n, m);
}

int TNeedlemanWunsch::FMakeMatrix(){
	// Init Matrix
	for(unsigned i=0; i<n; i++)	this->m_dp[i][0] = -i*this->gap_score;
	for(unsigned j=0; j<m; j++)	this->m_dp[0][j] = -j*this->gap_score;

	int tmp;
	int mtmp;

	for(unsigned i=1; i<n; i++){
		for(unsigned j=1; j<m; j++){
			tmp = FSimilarity(m_dna[0][i-1], m_dna[1][j-1]);

			mtmp = std::max(m_dp[i-1][j]-gap_score, m_dp[i][j-1]-gap_score);
			this->m_dp[i][j] = std::max(m_dp[i-1][j-1]+tmp, mtmp);
		}
	}

	return this->m_dp[n-1][m-1];
}

std::pair<std::string, std::string> TNeedlemanWunsch::FGlobalOptimum(){
	int i = n-1;
	int j = m-1;

	std::string align_a = "";
	std::string align_b = "";

	while(i!=0 || j!=0){
		if(i==0){
			align_a = '-' + align_b;
			align_b = m_dna[1][j-1] + align_b;

			j--;
		}
		else if(j==0){
			align_a = m_dna[0][i-1] + align_a; 
			align_b = '-' + align_b;

			i--;
		}
		else{
			int s = (m_dna[0][i-1] == m_dna[1][j-1]) ? match_score: -mismatch_score;

			if(m_dp[i][j] == m_dp[i-1][j-1]+s){
				align_a = m_dna[0][i-1] + align_a;
				align_b = m_dna[1][j-1] + align_b;

				i--;
				j--;
			}
			else if(m_dp[i-1][j] > m_dp[i][j-1]){
				align_a = m_dna[0][i-1] + align_a;
				align_b = '-' + align_b;

				i--;
			}
			else{
				align_a = '-' + align_a;
				align_b = m_dna[1][j-1] + align_b;

				j--;
			}
		}
	}	

	return std::make_pair(align_a, align_b);
}

TNeedlemanWunsch::~TNeedlemanWunsch(){
	delete []this->m_dp;
}

#endif