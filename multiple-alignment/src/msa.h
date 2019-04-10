#ifndef _MSA_H_
#define _MSA_H_

#include "../../pairwise-alignment/src/needleman-wunsch.h"
#include "../../utils/score.h"
#include "../../utils/print.h"

class TMsa{
protected:
	TNeedlemanWunsch *m_nw;
	std::vector<std::string> m_dnas;
	std::vector<std::vector<TPairwiseAlignment> > m_matrix_alignments;

	int match_score;					// match score
	int mismatch_score;					// mismatch score
	int gap_score;						// gap score	

public:
	TMsa(int, int, int);
	virtual ~TMsa();

	virtual int  FSumPairs(std::vector<std::string >);
	virtual void FSetDna(std::vector<std::string>);
	virtual int  FSimilarity(char, char);
	virtual void FAligments();
	
	virtual unsigned FGetMaximunIdx();
};

TMsa::TMsa(int _match, int _mismatch, int _gap){
	this->match_score    = _match;
	this->mismatch_score = _mismatch;
	this->gap_score      = _gap;
	this->m_nw = new TNeedlemanWunsch(match_score, mismatch_score, gap_score);
}

void TMsa::FSetDna(std::vector<std::string> _dnas){
	this->m_dnas = _dnas;

	this->m_matrix_alignments = std::vector<std::vector<TPairwiseAlignment> >(m_dnas.size(),
		std::vector<TPairwiseAlignment>(m_dnas.size()));
}

int TMsa::FSimilarity(char _a, char _b){
	int tmp;

	if(_a=='-' or _b=='-')	tmp = -gap_score;
	else if(_a==_b)	tmp = match_score;
	else if(_a!=_b)	tmp = -mismatch_score;

	return tmp;
}

int TMsa::FSumPairs(std::vector<std::string > _alignments){
	int score = 0;
	int tmp;

	for(unsigned j=0; j<_alignments[0].size(); j++){
		tmp = 0;
		for(unsigned i=0; i<_alignments.size(); i++){
			for(unsigned k=i+1; k<_alignments.size(); k++){
				tmp += FSimilarity(_alignments[i][j], _alignments[k][j]);
			}
		}
		// std::cout << tmp << " ";
		score += tmp;
	}
	// std::cout << "\n";

	return score;
}

void TMsa::FAligments(){
	TPairwiseAlignment tmp;
	for(unsigned i=0; i<m_dnas.size(); i++){
		for(unsigned j=i+1; j<m_dnas.size(); j++){
			tmp = m_nw->FGetScoreAlignment(m_dnas[i], m_dnas[j]);
			this->m_matrix_alignments[i][j] = tmp;
			this->m_matrix_alignments[j][i] = TPairwiseAlignment(tmp.m_score,
				std::make_pair(tmp.m_alignment.second, tmp.m_alignment.first));
		}
	}
}

unsigned TMsa::FGetMaximunIdx(){
	unsigned idx;
	int maxi = 0;
	int tmp;

	for(unsigned j=0; j<m_matrix_alignments[0].size(); j++)
		maxi += m_matrix_alignments[0][j].m_score;
	idx = 0;

	for(unsigned i=1; i<m_matrix_alignments.size(); i++){
		tmp = 0;
		for(unsigned j=0; j<m_matrix_alignments[i].size(); j++){
			tmp += m_matrix_alignments[i][j].m_score;
		}

		if(tmp > maxi){
			maxi = tmp;
			idx = i;
		}
	}

	return idx;
}

TMsa::~TMsa(){
	m_dnas.clear();
	m_matrix_alignments.clear();
	delete m_nw;	
}

#endif