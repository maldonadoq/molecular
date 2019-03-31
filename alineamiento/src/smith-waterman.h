#ifndef _SMITH_WATERMAN_H_
#define _SMITH_WATERMAN_H_

#include <string.h>
#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include "alignment.h"

#define TMAX 1000
#define PATH 3

class TSmithWaterman: public TAlignment{
public:
	int m_score;
	TSmithWaterman(std::string _dnaa, std::string _dnab,int _match, int _mismatch, int _gap):
		TAlignment(_dnaa,_dnab,_match,_mismatch,_gap){	};
	~TSmithWaterman(){	};

	int FMakeMatrix();
	std::vector<std::pair<unsigned, unsigned> > FFindNMax(unsigned);
	std::vector<std::pair<std::string,std::string> > FLocalOptimum(unsigned);
	std::vector<std::vector<std::pair<int, int> > > FBFS(std::pair<unsigned, unsigned>, int);
};

std::vector<std::pair<unsigned, unsigned> > TSmithWaterman::FFindNMax(unsigned _n){
	std::vector<std::pair<unsigned, unsigned> > maxs;

	if(m_score > 0){
		for(unsigned i=1; i<n; i++){
			for(unsigned j=1; j<m; j++){
				if(m_dp[i][j] == m_score){
					maxs.push_back(std::make_pair(i,j));
					if(maxs.size() > _n){
						i = n;	j = m;
					}
				}
			}
		}
	}

	return maxs;
}

int TSmithWaterman::FMakeMatrix(){
	// Init Matrix
	this->m_dp[0][0] = 0;
	for(unsigned i=1; i<n; i++)	this->m_dp[i][0] = 0;
	for(unsigned j=1; j<m; j++)	this->m_dp[0][j] = 0;

	int tmp;
	int p,q,r,mx;

	this->m_score = 0;

	// Array [0,0,0]
	for(unsigned i=1; i<n; i++){
		for(unsigned j=1; j<m; j++){
			tmp = FSimilarity(m_dna[0][i-1], m_dna[1][j-1]);

			p = m_dp[i][j-1] - gap_score;		// horizontal 0
			q = m_dp[i-1][j-1] + tmp;			// diagonal   1
			r = m_dp[i-1][j] - gap_score;		// vertical	  2
			mx = std::max(p,std::max(q,std::max(r,0)));

			if(mx>0){
				if(p == mx)	this->m_back[i][j][0] = '1';
				if(q == mx)	this->m_back[i][j][1] = '1';
				if(r == mx)	this->m_back[i][j][2] = '1';
			}
			this->m_dp[i][j] = mx;
			if(mx > m_score)
				this->m_score = mx;
		}
	}

	return m_score;
}

std::vector<std::vector<std::pair<int, int> > > TSmithWaterman::FBFS(std::pair<unsigned, unsigned> _src, int _trg){

	std::vector<std::vector<std::pair<int, int> > > mpaths;
	std::queue<std::vector<std::pair<int, int> > >	mqueue;
	std::vector<std::pair<int, int> > path;
	std::vector<std::pair<int, int> > tpath;

	path.push_back(_src);
	mqueue.push(path);

	std::pair<int, int> last;
	std::pair<int, int> tmp;

 	while(!mqueue.empty()){
		path = mqueue.front();
		mqueue.pop();

		last = path[path.size()-1];
		if(m_dp[last.first][last.second] == _trg){
			mpaths.push_back(path);
		}
		else{
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

					if(is_not_visited(path, tmp)){
						tpath = path;
						tpath.push_back(tmp);
						mqueue.push(tpath);
					}
				}
			}
		}
	}

	return mpaths;
}

std::vector<std::pair<std::string,std::string> > TSmithWaterman::FLocalOptimum(unsigned _n){
	std::vector<std::pair<std::string, std::string> > alignments;
	std::vector<std::vector<std::pair<int, int> > > paths;
	std::vector<std::vector<std::pair<int, int> > > tpaths;

	std::vector<std::pair<unsigned, unsigned> > maxs = FFindNMax(_n);
	std::vector<std::pair<int, int> > tpath;

	for(unsigned i=0; i<maxs.size(); i++){
		tpaths = FBFS(maxs[i], 0);
		for(unsigned j=0; j<tpaths.size(); j++){
			paths.push_back(tpaths[j]);
		}
	}

	std::pair<std::string, std::string> alignm;
	std::pair<unsigned, unsigned> tmp;

	unsigned ii;
	unsigned jj;

	for(unsigned i=0; i<paths.size(); i++){
		alignm.first  = "";
		alignm.second = "";

		tmp = paths[i][paths[i].size()-1];
		ii = tmp.first;
		jj = tmp.second;
		
		for(int j=paths[i].size()-2; j>=0; j--){
			tmp = paths[i][j];
			if(tmp.first == ii) alignm.first += '-';
			else alignm.first += m_dna[0][tmp.first-1];

			if(tmp.second == jj) alignm.second += '-';
			else alignm.second += m_dna[1][tmp.second-1];

 			ii = tmp.first;	
 			jj = tmp.second;
		}
		alignments.push_back(make_pair(alignm.first, alignm.second));		
	}

	paths.clear();

	return alignments;
}

#endif