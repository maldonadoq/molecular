#ifndef _SMITH_WATERMAN_H_
#define _SMITH_WATERMAN_H_

extern unsigned TMAX;

#include <utility>
#include "func.h"

class TSmithWaterman{
private:
	std::string m_dna[2];		// dna strings
	unsigned n, m;				// matrix (row-col)
	int match_score;			// match score
	int mismatch_score;			// mismatch score
	int gap_score;				// gap score

public:
	int **m_dp;					// matrix
	TSmithWaterman(std::string, std::string, int, int, int);
	~TSmithWaterman();

	void FMakeMatrix();
	void FPrintMatrix();
	int FSimilarity(char, char);
	std::pair<int, int> FFindMax();
	std::pair<std::string, std::string> FLocalOptimum();
};

TSmithWaterman::TSmithWaterman(std::string _dnaa, std::string _dnab,
	int _match, int _mismatch, int _gap){
	this->m_dna[0] = _dnaa;
	this->m_dna[1] = _dnab;

	this->n = this->m_dna[0].size()+1;
	this->m = this->m_dna[1].size()+1;

	this->match_score    = _match;
	this->mismatch_score = _mismatch;
	this->gap_score 	 = _gap;

	if((this->n <  TMAX) and (this->m < TMAX)){
		this->m_dp = new int*[this->n];
		for(unsigned i=0; i<this->n; i++){
			this->m_dp[i] = new int[this->m];
		}
	}
}

int TSmithWaterman::FSimilarity(char _a, char _b){
	return (_a == _b)? match_score: -mismatch_score;
	// return (_a == _b)? match_score: -gap_score;
}

void TSmithWaterman::FPrintMatrix(){
	print_matrix(m_dp, n, m);
}

std::pair<int, int> TSmithWaterman::FFindMax(){
	int tmax = 0;
	int ii, jj;
	for(unsigned i=1; i<n-1; i++){
		for(unsigned j=1; j<m-1; j++){
			if(m_dp[i][j] > tmax){
				tmax = m_dp[i][j];
				ii = i;
				jj = j;
			}
		}
	}

	return std::make_pair(ii,jj);
}

void TSmithWaterman::FMakeMatrix(){
	for(unsigned i=0; i<n; i++)	this->m_dp[i][0] = 0;
	for(unsigned j=0; j<m; j++)	this->m_dp[0][j] = 0;

	int tmp, tmp1, tmp2;
	for(unsigned i=1; i<n; i++){
		for(unsigned j=1; j<m; j++){
			tmp  = FSimilarity(m_dna[0][i-1], m_dna[1][j-1]);
			tmp1 = std::max(m_dp[i-1][j]-gap_score, m_dp[i][j-1]-gap_score);
			tmp2 = std::max(m_dp[i-1][j-1] + tmp, tmp1);
			this->m_dp[i][j] = std::max(tmp2, 0);
		}
	}
}

std::pair<std::string, std::string> TSmithWaterman::FLocalOptimum(){
	std::pair<int, int> idx = FFindMax();

	int i = idx.first;
	int j = idx.second;

	std::string align_a = "";
	std::string align_b = "";

	while((i!=0 || j!=0) and m_dp[i][j] != 0){
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

TSmithWaterman::~TSmithWaterman(){
	delete []m_dp;
}

#endif