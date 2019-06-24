#ifndef _POINTING_MATRIX_H_
#define _POINTING_MATRIX_H_

#include <iostream>
#include <vector>
#include <string>
#include <utility>

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

	int match_score;					// match score
	int mismatch_score;					// mismatch score
	int gap_score;						// gap score

	vector<vector<char> > m_pmatrix;	// pointing matrix
	vector<vector<int> >  m_smatrix;	// score matrix
public:
	PMatrixAlignment(std::string, std::string, int, int, int);
	PMatrixAlignment();
	~PMatrixAlignment();

	int  similarity(char, char);
	void formation();
	void alignment(string &, string &);
	void print();
	void init();

	pair<string, string> run();
};

PMatrixAlignment::PMatrixAlignment(){

}

PMatrixAlignment::PMatrixAlignment(std::string _dnaa, std::string _dnab,
	int _match, int _mismatch, int _gap){	
	
	this->match_score    = _match;
	this->mismatch_score = _mismatch;
	this->gap_score      = _gap;

	this->m_dna[0] = _dnaa;
	this->m_dna[1] = _dnab;

	init();
}

void PMatrixAlignment::init(){
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
	float mx;			// max temporal
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

	int score = 0.0;
	int row = m_dna[1].size();
	int col = m_dna[0].size();

	while(m_pmatrix[row][col] != zero){

		if(m_pmatrix[row][col] == horizontal){
			al1 = m_dna[0][col-1] + al1;
			al2 = "-" + al2;

			col--;
			score--;
		}
		else if(m_pmatrix[row][col] == vertical){
			al1 = "-" + al1;
			al2 = m_dna[1][row-1] + al2;

			row--;
			score--;
		}
		else if(m_pmatrix[row][col] == diagonal){
			al1 = m_dna[0][col-1] + al1;
			al2 = m_dna[1][row-1] + al2;

			if(m_dna[0][col-1] == m_dna[1][row-1])
				score++;
			else
				score--;

			row--;
			col--;
		}
	}

	cout << "Score: " << score << "\n";
}

pair<string, string> PMatrixAlignment::run(){
	formation();
	string al1, al2;
	alignment(al1, al2);

	return make_pair(al1, al2);
}

void PMatrixAlignment::print(){
	cout << "Pointing Matrix\n";
	print_matrix(m_pmatrix);

	cout << "\nScore Matrix\n";
	print_matrix(m_smatrix);
}

PMatrixAlignment::~PMatrixAlignment(){
	m_pmatrix.clear();
	m_smatrix.clear();
}

#endif
