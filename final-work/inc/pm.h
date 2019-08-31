#ifndef _PM_H_
#define _PM_H_

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

enum{
	horizontal	= '3',
	vertical	= '1',
	diagonal	= '2',
	zero		= '0'
};

class PMatrixAlignment{
private:
	vector<vector<char> > m_pmatrix;	// pointing matrix
	vector<vector<int> >  m_smatrix;	// score matrix
	string m_dna[2];					// dna string

	int match_score;					// match score
	int mismatch_score;					// mismatch score
	int gap_score;						// gap score
	int score;
public:
	PMatrixAlignment(int, int, int);
	PMatrixAlignment();
	~PMatrixAlignment();

	int  similarity(char, char);
	void formation();
	void alignment(string &, string &);
	void init(string, string);
	TItem run(string, string);
};

PMatrixAlignment::PMatrixAlignment(){

}

PMatrixAlignment::PMatrixAlignment(int _match, int _mismatch, int _gap){	
	this->match_score    = _match;
	this->mismatch_score = _mismatch;
	this->gap_score      = _gap;
}

void PMatrixAlignment::init(string _dnaa, string _dnab){
	this->m_dna[0] = _dnaa;
	this->m_dna[1] = _dnab;

	this->m_smatrix.clear();
	this->m_pmatrix.clear();
	this->m_pmatrix = vector<vector<char> >(m_dna[1].size()+1, vector<char>(m_dna[0].size()+1));
	this->m_smatrix = vector<vector<int> >(2, vector<int>(m_dna[0].size()+1));
	
	unsigned i;
	for(i=0; i<m_pmatrix.size(); i++){
		m_pmatrix[i][0] = vertical;
	}

	for(i=0; i<m_pmatrix[0].size(); i++){
		m_pmatrix[0][i] = horizontal;
		m_smatrix[0][i] = (int)i*gap_score;
	}

	m_pmatrix[0][0] = zero;
}

int PMatrixAlignment::similarity(char _a, char _b){
	return (_a == _b)? match_score: mismatch_score;
}

void PMatrixAlignment::formation(){
	int tor, tcr, tmp;	// old row and current row

	unsigned i, j;		// idx
	int mx;				// max temporal
	char pointer;		// come from
	int cs;				// similarity

	tor = 0;
	tcr = 1;
	for(i=1; i<m_pmatrix.size(); i++){
		m_smatrix[tor][0] = int(i-1)*gap_score;
		m_smatrix[tcr][0] = int(i)*gap_score;

		for(j=1; j<m_pmatrix[i].size(); j++){
			cs = similarity(m_dna[1][i-1], m_dna[0][j-1]);
			mx = m_smatrix[tor][j-1] + cs;
			pointer = diagonal;

			if(mx < m_smatrix[tor][j] + gap_score){
				mx = m_smatrix[tor][j] + gap_score;
				pointer = vertical;
			}

			if(mx < m_smatrix[tcr][j-1] + gap_score){
				mx = m_smatrix[tcr][j-1] + gap_score;
				pointer = horizontal;
			}

			m_smatrix[tcr][j] = mx;
			m_pmatrix[i][j] = pointer;
		}
		tmp = tor;
		tor = tcr;
		tcr = tmp;
	}
}

void PMatrixAlignment::alignment(string &al1, string &al2){
	al1 = "";
	al2 = "";

	int row = m_dna[1].size();
	int col = m_dna[0].size();

	while(m_pmatrix[row][col] != zero){
		if(m_pmatrix[row][col] == diagonal){
			al1 = m_dna[0][col-1] + al1;
			al2 = m_dna[1][row-1] + al2;

			row--;
			col--;
		}
		else if(m_pmatrix[row][col] == horizontal){
			al1 = m_dna[0][col-1] + al1;
			al2 = "-" + al2;

			col--;
		}
		else if(m_pmatrix[row][col] == vertical){
			al1 = "-" + al1;
			al2 = m_dna[1][row-1] + al2;

			row--;
		}
	}
}

TItem PMatrixAlignment::run(string _dnaa, string _dnab){
	init(_dnaa, _dnab);
	formation();
	string al1, al2;
	alignment(al1, al2);

	return {al1, al2, score};
}

PMatrixAlignment::~PMatrixAlignment(){
	m_pmatrix.clear();
	m_smatrix.clear();
}

#endif