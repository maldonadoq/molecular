#ifndef _NEEDLEMAN_WUNSCH_H_
#define _NEEDLEMAN_WUNSCH_H_

#include <string.h>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <list>
#include "func.h"

#define TMAX 1000
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
	unsigned m_qp;						// number to stop recursion!!
public:	
	int  **m_dp;					// matrix
	char ***m_back;				// matrix to do the traceback

	TNeedlemanWunsch(std::string, std::string, int, int, int);
	~TNeedlemanWunsch();			
	
	int  FMakeMatrix();
	void FGlobalOptimum(unsigned);
	void FGlobalOptimum(std::pair<int, int>, std::pair<int,int>, bool **, std::vector<std::pair<int, int> > &,
		std::vector<std::vector<std::pair<int, int> > > &, bool &);
	void FPrintBackMatrix();
	int  FGetNumberAlignment();
	void FPrintWeightMatrix();
	int  FSimilarity(char, char);

	std::string FGetLargerSequence();
	std::vector<std::string> FGetAlignment();
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

		this->m_qp	= 0;
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

void TNeedlemanWunsch::FGlobalOptimum(unsigned _n){
	std::pair<int, int> src = std::make_pair(n-1,m-1);
	std::pair<int, int> trg = std::make_pair(0,0);	

	this->m_qp = _n;

	// pull boolean matrix [m][n]
	bool **visited = new bool*[n];
	for(unsigned i=0; i<n; i++){
		visited[i] = new bool[m];
		for(unsigned j=0; j<m; j++)
			visited[i][j] = false;
	}

	std::vector<std::pair<int, int> > mpath;
	std::vector<std::vector<std::pair<int, int> > > mpaths;
	bool end = false;
	FGlobalOptimum(src, trg, visited, mpath, mpaths, end);

	for(unsigned i=0; i<mpaths.size(); i++)
		print_vector_pair(mpaths[i]);
}

void TNeedlemanWunsch::FGlobalOptimum(std::pair<int, int> _src, std::pair<int, int> _trg,
	bool ** _visited, std::vector<std::pair<int, int> > &_path,
	std::vector<std::vector<std::pair<int, int> > > &_mpaths, bool &_end){

	_visited[_src.first][_src.second] = true;
	_path.push_back(_src);

	if(_src == _trg){
		_mpaths.push_back(_path);
		if(_mpaths.size()>=this->m_qp)
			_end = true;
	}
	else{
		std::pair<int, int> tmp;
		for(unsigned i=0; i<PATH; i++){
			if(m_back[_src.first][_src.second][i] == '1' and !_end){
				switch(i){
					case 0:
						tmp = std::make_pair(_src.first,   _src.second-1);
						break;
					case 1:
						tmp = std::make_pair(_src.first-1, _src.second-1);
						break;
					case 2:
						tmp = std::make_pair(_src.first-1, _src.second);
						break;
				}
				if(tmp.first>=0 and tmp.second>=0){
					if(!_visited[tmp.first][tmp.second]){
						FGlobalOptimum(tmp, _trg, _visited, _path, _mpaths, _end);
					}
				}
			}
		}
	}

	_path.pop_back();
	_visited[_src.first][_src.second] = false;
}

int TNeedlemanWunsch::FGetNumberAlignment(){
	return this->m_align.size();
}

std::vector<std::string> TNeedlemanWunsch::FGetAlignment(){
	return this->m_align;
}


TNeedlemanWunsch::~TNeedlemanWunsch(){
	delete []this->m_dp;
}

#endif
