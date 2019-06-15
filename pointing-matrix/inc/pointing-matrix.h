#ifndef _POINTING_MATRIX_H_
#define _POINTING_MATRIX_H_

#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::cout;

enum{
	horizontal	= 3,
	vertical	= 1,
	diagonal	= 2
};

void print_matrix(vector<vector<float> > _matrix){
	unsigned i,j;
	for(i=0; i<_matrix.size(); i++){
		for(j=0; j<_matrix[i].size(); j++){
			cout << _matrix[i][j] << "\t";
		}
		cout << "\n";
	}
}

class PMatrixAlignment{
private:
	string m_dna[2];					// dna string

	int match_score;					// match score
	int mismatch_score;					// mismatch score
	int gap_score;						// gap score

	vector<vector<float> > m_pmatrix;	// pointing matrix
	vector<vector<float> > m_smatrix;	// score matrix
public:
	PMatrixAlignment(std::string, std::string, int, int, int);
	PMatrixAlignment();
	~PMatrixAlignment();

	int  similarity(char, char);
	void formation();
	void alignment();
	void print();
	void init();
	void run();
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
	this->m_pmatrix = vector<vector<float> >(m_dna[1].size()+1, vector<float>(m_dna[0].size()+1));
	this->m_smatrix = vector<vector<float> >(2, vector<float>(m_dna[0].size()+1));
	
	unsigned i;
	for(i=0; i<m_pmatrix.size(); i++){
		m_pmatrix[i][0] = vertical;
	}

	for(i=0; i<m_pmatrix[0].size(); i++){
		m_pmatrix[0][i] = horizontal;
		m_smatrix[0][i] = (int)i*gap_score;
	}

	m_pmatrix[0][0] = 0;
}

int PMatrixAlignment::similarity(char _a, char _b){
	return (_a == _b)? match_score: mismatch_score;
}

void PMatrixAlignment::formation(){
	int tor, tcr, tmp;	// old row and current row

	unsigned i, j;		// idx
	float mx;			// max temporal
	int pointer;		// come from
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

void PMatrixAlignment::alignment(){
	string al1 = "";
	string al2 = "";

	float score = 0.0;
	int row = m_dna[1].size();
	int col = m_dna[0].size();

	while(m_pmatrix[row][col] != 0){
		if(m_pmatrix[row][col] == 3){
			al1 += m_dna[1][row-1];
			al2 += "-";
			row--;
			score = score - 1;
		}
		else if(m_pmatrix[row][col] == 1){
			al1 += "-";
			al2 += m_dna[0][col-1];
			col--;
			score = score - 1;
		}
		else if(m_pmatrix[row][col] == 2){
			al1 += m_dna[1][row-1];
			al2 += m_dna[0][col-1];

			if(m_dna[0][col-1] == m_dna[1][row-1]){
				score = score + 1;
			}
			else{
				score = score - 1;
			}

			row--;
			col--;
		}
	}

	cout << "al1: " << al1 << "\n";
	cout << "al2: " << al2 << "\n";
}

void PMatrixAlignment::run(){
	formation();
	// print();
	// alignment();
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