#ifndef _NEEDLEMAN_WUNSCH_H_
#define _NEEDLEMAN_WUNSCH_H_

#include <string.h>
#include <iostream>
#include <utility>
#include <vector>
#include "alignment.h"
#include "../../utils/score.h"

#define TMAX 1000
#define PATH 3

class TNeedlemanWunsch: public TAlignment{
public:
	unsigned m_qp;
	TNeedlemanWunsch(std::string _dnaa, std::string _dnab,
	int _match, int _mismatch, int _gap){
		this->m_qp = 0;

		this->match_score    = _match;
		this->mismatch_score = _mismatch;
		this->gap_score      = _gap;
		
		FInit(_dnaa, _dnab);
	}

	TNeedlemanWunsch(int _match, int _mismatch, int _gap): TAlignment(_match, _mismatch, _gap){	}
	TNeedlemanWunsch(): TAlignment(){	};
	~TNeedlemanWunsch(){	};

	void FInit(std::string, std::string);
	int  FMakeMatrix();
	std::vector<std::pair<std::string,std::string> > FGlobalOptimum(unsigned);
	void FDFS(std::pair<int, int>, std::pair<int,int>, std::vector<std::pair<int, int> > &,
		std::vector<std::vector<std::pair<int, int> > > &, bool &);

	TPairwiseAlignment FGetScoreAlignment(std::string, std::string);
};

void TNeedlemanWunsch::FInit(std::string _dnaa, std::string _dnab){
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
				this->m_back[i][j] = new char[PATH+1];
				for(unsigned k=0; k<PATH+1; k++){
					this->m_back[i][j][k] = '0';
				}
			}
		}
	}
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

TPairwiseAlignment TNeedlemanWunsch::FGetScoreAlignment(std::string _dnaa, std::string _dnab){
	FInit(_dnaa, _dnab);
	int tscore = FMakeMatrix();
	std::vector<std::pair<std::string,std::string> > talignments = FGlobalOptimum(1);

	delete []this->m_dp;
	delete []this->m_back;
	return TPairwiseAlignment(tscore, talignments[0]);
}

std::vector<std::pair<std::string,std::string> > TNeedlemanWunsch::FGlobalOptimum(unsigned _n){

	std::pair<int, int> src = std::make_pair(n-1,m-1);
	std::pair<int, int> trg = std::make_pair(0,0);	

	this->m_qp = _n;

	std::vector<std::pair<int, int> > mpath;
	std::vector<std::vector<std::pair<int, int> > > mpaths;
	bool end = false;
	FDFS(src, trg, mpath, mpaths, end);

	std::vector<std::pair<std::string, std::string> > alignments;
	std::pair<std::string, std::string> alignm;
	std::pair<unsigned, unsigned> tmp;
	
	unsigned ii;
	unsigned jj;

	for(unsigned i=0; i<mpaths.size(); i++){
		alignm.first  = "";
		alignm.second = "";

		ii = 0;
		jj = 0;
		
		for(int j=mpaths[i].size()-2; j>=0; j--){
			tmp = mpaths[i][j];

			alignm.first += (tmp.first == ii)? '-': m_dna[0][tmp.first-1];
			alignm.second += (tmp.second == jj)? '-': m_dna[1][tmp.second-1];

 			ii = tmp.first;	
 			jj = tmp.second;
		}
		// std::cout << "\n";
		alignments.push_back(make_pair(alignm.first, alignm.second));		
	}

	mpaths.clear();

	return alignments;
}

void TNeedlemanWunsch::FDFS(std::pair<int, int> _src, std::pair<int, int> _trg,
	std::vector<std::pair<int, int> > &_path,
	std::vector<std::vector<std::pair<int, int> > > &_mpaths, bool &_end){

	m_back[_src.first][_src.second][PATH] = '1';
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
				tmp = std::make_pair(_src.first+DIRECTION[i][0], _src.second+DIRECTION[i][1]);

				if(m_back[tmp.first][tmp.second][PATH] == '0'){
					FDFS(tmp, _trg, _path, _mpaths, _end);
				}
			}
		}
	}

	_path.pop_back();
	m_back[_src.first][_src.second][PATH] = '0';
}

#endif
