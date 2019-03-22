#ifndef _NEEDLEMAN_WUNSCH_H_
#define _NEEDLEMAN_WUNSCH_H_

#include <string.h>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <queue>
#include "func.h"

#define TMAX 100
#define PATH 3


/*	
	1   2
	 \  |
	  \ |
	0---1
*/


class TNeedlemanWunsch{
private:
	std::string m_dna[2];				// dna string
	unsigned n, m;						// matrix
	int match_score;					// match score
	int mismatch_score;					// mismatch score
	int gap_score;						// gap score
	std::vector<std::string> m_align;	// all aligments
public:	
	int **m_dp;					// matrix
	char ***m_back;				// matrix to do the traceback

	TNeedlemanWunsch(std::string, std::string, int, int, int);
	~TNeedlemanWunsch();			
	
	int  FMakeMatrix();
	void FGlobalOptimum();
	void FPrintBackMatrix();
	int  FGetTotalAlignment();
	void FPrintWeightMatrix();
	int  FSimilarity(char, char);

	std::string FGetLargerSequence();
	std::vector<std::string> FGetAllAlignment();
	std::vector<std::string> FGetNAlignment(unsigned);
};

TNeedlemanWunsch::TNeedlemanWunsch(std::string _dnaa, std::string _dnab,
	int _match, int _mismatch, int _gap){	

	if((_dnaa.size() <  TMAX) and (_dnab.size() < TMAX)){
		this->m_dna[0] = (_dnaa.size()>_dnab.size())? _dnaa: _dnab;
		this->m_dna[1] = (_dnaa.size()<_dnab.size())? _dnaa: _dnab;

		this->n = m_dna[0].size()+1;
		this->m = m_dna[1].size()+1;

		this->match_score    = _match;
		this->mismatch_score = _mismatch;
		this->gap_score      = _gap;

		this->m_align.clear();
	
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

int TNeedlemanWunsch::FSimilarity(char _a, char _b){
	return (_a == _b)? match_score: -mismatch_score;
}

void TNeedlemanWunsch::FPrintWeightMatrix(){
	print_matrix(m_dp, n, m);
}

void TNeedlemanWunsch::FPrintBackMatrix(){
	print_matrix(m_back, n, m, PATH);
}

int TNeedlemanWunsch::FMakeMatrix(){
	// Init Matrix
	this->m_dp[0][0] = 0;
	for(unsigned i=1; i<n; i++){
		this->m_dp[i][0] = -i*this->gap_score;
		this->m_back[i][0][2] = '1';
	}
	for(unsigned j=1; j<m; j++){
		this->m_dp[0][j] = -j*this->gap_score;
		this->m_back[0][j][0] = '1';
	}

	int tmp;
	int p,q,r,mx;

	// Array [0,0,0]
	for(unsigned i=1; i<n; i++){
		for(unsigned j=1; j<m; j++){
			tmp = FSimilarity(m_dna[0][i-1], m_dna[1][j-1]);

			p = m_dp[i][j-1] - gap_score;		// horizontal 0
			q = m_dp[i-1][j-1] + tmp;			// diagonal   1
			r = m_dp[i-1][j] - gap_score;		// vertical	  2
			mx = std::max(p,std::max(q,r));

			if(p == mx)	this->m_back[i][j][0] = '1';
			if(q == mx)	this->m_back[i][j][1] = '1';
			if(r == mx)	this->m_back[i][j][2] = '1';
			
			this->m_dp[i][j] = mx;
		}
	}

	return this->m_dp[n-1][m-1];
}

std::string TNeedlemanWunsch::FGetLargerSequence(){
	return m_dna[0];
}

void TNeedlemanWunsch::FGlobalOptimum(){
	std::pair<int, int> src = std::make_pair(n-1,m-1);
	std::pair<int, int> trg = std::make_pair(0,0);	

	std::queue<std::vector<std::pair<int, int> > >	mqueue;
	std::vector<std::pair<int, int> >				mpath;

	mpath.push_back(src);
	mqueue.push(mpath);

	std::pair<int, int> last;
	std::pair<int, int> tmp;

	std::vector<std::vector<std::pair<int, int> > >	malignment;

	while(!mqueue.empty()){
		mpath = mqueue.front();
		mqueue.pop();

		last = mpath[mpath.size()-1];		

		if(last == trg){
			// print_vector_pair(mpath);
			malignment.push_back(mpath);
		}

		if(last.first>0 or last.second>0){
			for(unsigned i=0; i<PATH; i++){
				if(m_back[last.first][last.second][i] == '1'){
					switch(i){
						case 0:
							tmp = std::make_pair(last.first,   last.second-1);
							break;
						case 1:
							tmp = std::make_pair(last.first-1, last.second-1);
							break;
						case 2:
							tmp = std::make_pair(last.first-1, last.second);
							break;
					}

					if(is_not_visited(mpath, tmp)){
						std::vector<std::pair<int, int> > tpath(mpath);
						tpath.push_back(tmp);
						mqueue.push(tpath);
					}
				}
			}
		}
	}

	int k;

	std::string dna = m_dna[1];
	std::string alignm;

	for(unsigned i=0; i<malignment.size(); i++){
		alignm = "";
		k = 0;

		for(int j=malignment[i].size()-2; j>=0 ; j--){
			tmp = malignment[i][j];			
			if(tmp.second == k)		alignm += '-';
			else					alignm += dna[tmp.second-1];

			k = tmp.second;
		}

		this->m_align.push_back(alignm);
	}
}

int TNeedlemanWunsch::FGetTotalAlignment(){
	return this->m_align.size();
}

std::vector<std::string> TNeedlemanWunsch::FGetAllAlignment(){
	return this->m_align;
}

std::vector<std::string> TNeedlemanWunsch::FGetNAlignment(unsigned _n){
	if(_n < this->m_align.size()){
		std::vector<std::string> tmp;
		for(unsigned i=0; i<_n; i++)
			tmp.push_back(m_align[i]);

		return tmp;
	}

	return this->m_align;
}

TNeedlemanWunsch::~TNeedlemanWunsch(){
	delete []this->m_dp;
}

#endif