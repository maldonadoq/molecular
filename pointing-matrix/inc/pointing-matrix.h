#ifndef _POINTING_MATRIX_H_
#define _POINTING_MATRIX_H_

#include <iostream>
#include <vector>
#include <string>
#include <utility>

#include "../../utils/score.h"
#include "../../utils/print.h"

using std::vector;
using std::string;
using std::cout;

using std::make_pair;
using std::pair;

enum{
	horizontal	= '3',
	vertical	= '1',
	diagonal	= '2',
	zero 		= '0'
};

class PMatrixAlignment{
private:
	string m_dna[2];					// dna string
	unsigned n, m;						// matrix
	int match_score;					// match score
	int mismatch_score;					// mismatch score
	int gap_score;						// gap score
	int score;							// final score
public:
	vector<vector<char> > m_pmatrix;
	int  ** m_smatrix;	// score matrix

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

	this->m_pmatrix = vector<vector<char> >(n, vector<char>(m));

	this->m_smatrix = new int*[2];			// score [int 4byte]
	this->m_smatrix[0] = new int[m];
	this->m_smatrix[1] = new int[m];
	
	unsigned i;
	for(i=0; i<m_pmatrix.size(); i++){
		m_pmatrix[i][0] = vertical;
	}

	for(i=0; i<m_pmatrix[0].size(); i++){
		m_pmatrix[0][i] = horizontal;
		m_smatrix[0][i] = (int)i*gap_score;
	}

	m_pmatrix[0][0] = zero;
	// print_matrix(m_pmatrix, n, m);
}

int PMatrixAlignment::similarity(char _a, char _b){
	return (_a == _b)? match_score: mismatch_score;
}

void PMatrixAlignment::formation(){
	int tor, tcr, tmp;	// old row and current row

	unsigned i, j;		// idx
	float mx;			// max temporal
	char pointer;		// come from
	int cs;				// similarity

	tor = 0;
	tcr = 1;
	for(i=1; i<m_pmatrix.size(); i++){
		m_smatrix[tor][0] = (int)(i-1)*gap_score;
		m_smatrix[tcr][0] = (int)i*gap_score;

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

	score = mx;
}

void PMatrixAlignment::alignment(string &al1, string &al2){
	al1 = "";
	al2 = "";

	int row = m_dna[1].size();
	int col = m_dna[0].size();

	while(m_pmatrix[row][col] != zero){

		if(m_pmatrix[row][col] == horizontal){
			al1 = m_dna[0][col-1] + al1;
			al2 = "-" + al2;

			col--;
		}
		else if(m_pmatrix[row][col] == vertical){
			al1 = "-" + al1;
			al2 = m_dna[1][row-1] + al2;

			row--;
		}
		else if(m_pmatrix[row][col] == diagonal){
			al1 = m_dna[0][col-1] + al1;
			al2 = m_dna[1][row-1] + al2;

			row--;
			col--;
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
	this->m_pmatrix.clear();
	this->m_smatrix = NULL;
}

#endif