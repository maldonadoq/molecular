#ifndef _NW_H_
#define _NW_H_

#include <iostream>
#include <vector>
#include <string>
#include <utility>

#include "../../utils/score.h"

using std::vector;
using std::string;
using std::cout;

using std::make_pair;
using std::pair;

class TNeedlemanWunsch{
private:
	vector<vector<int> > m_smatrix;	// score matrix
	string m_dna[2];			// dna string

	int match_score;			// match score
	int mismatch_score;			// mismatch score
	int gap_score;				// gap score
	int score;
public:	

	TNeedlemanWunsch();
 	TNeedlemanWunsch(int, int, int);
	~TNeedlemanWunsch();

	int  similarity(char, char);
	void formation();
	void alignment(string &, string &);
	void init(string, string);
	TItem run(string, string);
};

TNeedlemanWunsch::TNeedlemanWunsch(){

}

TNeedlemanWunsch::TNeedlemanWunsch(int _match, int _mismatch, int _gap){
 	this->match_score    = _match;
	this->mismatch_score = _mismatch;
	this->gap_score      = _gap;
}

void TNeedlemanWunsch::init(string _dnaa, string _dnab){
	this->m_dna[0] = _dnaa;
	this->m_dna[1] = _dnab;

 	this->m_smatrix = vector<vector<int> >(m_dna[1].size()+1, vector<int>(m_dna[0].size()+1));

 	// Init Matrix
	unsigned i, j;
	for(i=0; i<m_smatrix.size(); i++)
		this->m_smatrix[i][0] = i*this->gap_score;

	for(j=0; j<m_smatrix[0].size(); j++)
		this->m_smatrix[0][j] = j*this->gap_score;
}

int TNeedlemanWunsch::similarity(char _a, char _b){
	return (_a == _b)? match_score: mismatch_score;
}

void TNeedlemanWunsch::formation(){
	unsigned i,j;
 	int tmp, mtmp;

 	for(i=1; i<m_smatrix.size(); i++){
		for(j=1; j<m_smatrix[i].size(); j++){
			tmp = (m_dna[0][j-1] == m_dna[1][i-1])
				? match_score: mismatch_score;

 			mtmp = std::max(m_smatrix[i-1][j] + gap_score, m_smatrix[i][j-1] + gap_score);
			m_smatrix[i][j] = std::max(m_smatrix[i-1][j-1]+tmp, mtmp);
		}
	}

 	score = m_smatrix[m_smatrix.size()-1][m_smatrix[0].size()-1];
}

void TNeedlemanWunsch::alignment(string &al1, string &al2){
	al1 = "";
	al2 = "";

	int i = m_dna[1].size();
	int j = m_dna[0].size();
	int s;
	
 	while(i>0  and j>0){
		s = (m_dna[0][j-1] == m_dna[1][i-1]) ? match_score: mismatch_score;
		if(m_smatrix[i][j] == (m_smatrix[i-1][j-1] + s)){
			al1 = m_dna[0][j-1] + al1;
			al2 = m_dna[1][i-1] + al2;

			i--;
			j--;
		}
		else if((m_smatrix[i][j] - gap_score) == m_smatrix[i][j-1]){
			al1 = m_dna[0][j-1] + al1;
			al2 = '-' + al2;

			j--;
		}
		else if((m_smatrix[i][j] - gap_score) == m_smatrix[i-1][j]){
			al1 = '-' + al1;
			al2 = m_dna[1][i-1] + al2;

			i--;
		}
	}

	while(i>0){
		al1 = '-' + al1;
		al2 = m_dna[1][i-1] + al2;
		i--;
	}

	while(j>0){
		al1 = m_dna[0][j-1] + al1;
		al2 = '-' + al2;
		j--;
	}
}

TItem TNeedlemanWunsch::run(string _dnaa, string _dnab){
	init(_dnaa, _dnab);
	formation();
	string al1, al2;
	alignment(al1, al2);

	return {al1, al2, score};
}

TNeedlemanWunsch::~TNeedlemanWunsch(){
	this->m_smatrix.clear();
}

 #endif 