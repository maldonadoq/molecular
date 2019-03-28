#ifndef _NEEDLEMAN_WUNSCH_H_
#define _NEEDLEMAN_WUNSCH_H_

#include <string.h>
#include <iostream>
#include <utility>
#include <vector>
#include "alignment.h"

#define TMAX 1000
#define PATH 3

class TNeedlemanWunsch: public TAlignment{
public:
	unsigned m_qp;
	TNeedlemanWunsch(std::string _dnaa, std::string _dnab,
	int _match, int _mismatch, int _gap): TAlignment(_dnaa,_dnab,_match,_mismatch,_gap){
		this->m_qp = 0;
	}

	~TNeedlemanWunsch(){	};
	
	int  FMakeMatrix();
	std::vector<std::pair<std::string,std::string> > FGlobalOptimum(unsigned);
	void FDFS(std::pair<int, int>, std::pair<int,int>, bool **, std::vector<std::pair<int, int> > &,
		std::vector<std::vector<std::pair<int, int> > > &, bool &);
};

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

std::vector<std::pair<std::string,std::string> > TNeedlemanWunsch::FGlobalOptimum(unsigned _n){

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
	FDFS(src, trg, visited, mpath, mpaths, end);
	delete []visited;

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

			// std::cout << "[" << tmp.first << "," << tmp.second << "] ";

			if(tmp.first == ii) alignm.first += '-';
			else alignm.first += m_dna[0][tmp.first-1];

			if(tmp.second == jj) alignm.second += '-';
			else alignm.second += m_dna[1][tmp.second-1];

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
						FDFS(tmp, _trg, _visited, _path, _mpaths, _end);
					}
				}
			}
		}
	}

	_path.pop_back();
	_visited[_src.first][_src.second] = false;
}

#endif
